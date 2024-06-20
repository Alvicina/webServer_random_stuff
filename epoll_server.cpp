/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:06:03 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/18 11:12:31 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

#define PORT 8080
#define MAX_EVENTS 10
#define MAX_CLIENTS 100

struct vars
{
	int master_socket;
	int new_socket;
	int epoll_fd;
	struct sockaddr_in address;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];	
	int nfds;
	char buffer[1024];
	int bytes_read;
	int addrelen;
};

int createMasterSocket(struct vars *vars)
{
	vars->master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (vars->master_socket == -1)
	{
		perror("socket failed\n");
		return (-1);
	}
	return (0);
}

int confSocket(struct vars *vars)
{
	int option = 1;
	if (setsockopt(vars->master_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)))
	{
		perror("setsockopt");
		return (-1);
	}
	return (0);
}

void setTypeOfSocketCreated(struct vars *vars)
{
	vars->address.sin_family = AF_INET;
	vars->address.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	vars->address.sin_port = htons(PORT);
	vars->addrelen = sizeof(vars->addrelen);
}

int associateSocketWithPortIP(struct vars *vars)
{
	if (bind(vars->master_socket, (struct sockaddr *)&vars->address, sizeof(vars->address)) < 0)
	{
		perror("bind failed\n");
		close(vars->master_socket);
		return (-1);
	}
	return (0);
}

int listenForRequests(struct vars *vars)
{
	if (listen(vars->master_socket, 3) == -1)
	{
		perror("listen\n");
		close(vars->master_socket);
		return (-1);
	}
	return (0);
}

int instanceEpoll(struct vars *vars)
{
	vars->epoll_fd = epoll_create1(0);
	if (vars->epoll_fd == -1)
	{
		perror("epoll_create1");
		close(vars->master_socket);
		return (-1);
	}
	return (0);
}

int addMasterSocketToEpoll(struct vars *vars)
{
	vars->ev.events = EPOLLIN;
	vars->ev.data.fd = vars->master_socket;
	if (epoll_ctl(vars->epoll_fd, EPOLL_CTL_ADD, vars->master_socket, &vars->ev) == -1)
	{
		perror("epoll_ctl: master_socker\n");
		close(vars->master_socket);
		close(vars->epoll_fd);
		return (-1);
	}
	return (0);
}

int waitForConnection(struct vars *vars)
{
	vars->nfds = epoll_wait(vars->epoll_fd, vars->events, MAX_EVENTS, -1);
	if (vars->nfds == -1)
	{
		perror("epoll_wait\n");
		close(vars->master_socket);
		close(vars->epoll_fd);
		return (-1);;
	}
	return (0);
}

bool incomingNewConnection(struct vars *vars, int fdToCheck)
{
	if (vars->events[fdToCheck].data.fd == vars->master_socket)
		return (true);
	else
		return (false);
}

int acceptNewConnection(struct vars *vars)
{
	vars->new_socket = accept(vars->master_socket, (struct sockaddr *)&vars->address, (socklen_t *)&vars->addrelen);
	if (vars->new_socket == -1)
	{
		perror("accept\n");
		close(vars->master_socket);
		close(vars->epoll_fd);
		return (-1);
	}
	return (0);
}

int addNewSocketToEpoll(struct vars *vars)
{
	vars->ev.events = EPOLLIN | EPOLLET;
	vars->ev.data.fd = vars->new_socket;
	if (epoll_ctl(vars->epoll_fd, EPOLL_CTL_ADD, vars->new_socket, &vars->ev) == -1)
	{
		perror("epoll_ctl: new_socket\n");
		close(vars->new_socket);
		close(vars->master_socket);
		close(vars->epoll_fd);
		return (-1);
	}
	printf("New connection, socket fd is %d\n", vars->new_socket);
	return (0);
}

bool somethingToRead(struct vars *vars, int fdToCheck, int *check)
{
	vars->bytes_read = read(vars->events[fdToCheck].data.fd, vars->buffer, sizeof(vars->buffer) - 1);
	if (vars->bytes_read == -1)
	{
		*check = -1;
		return (false);
	}
	if (vars->bytes_read == 0)
		return (false);
	return (true);
}

void closeConnection(struct vars *vars, int fdToCheck)
{
	printf("Client disconnected, socket fd is %d\n", vars->events[fdToCheck].data.fd);
	close(vars->events[fdToCheck].data.fd);
}

void echoMessage(struct vars *vars, int fdToCheck)
{
	//(void) fdToCheck;
	char const *html = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 13\nConnection: close\n\nHello, Javi!";
	vars->buffer[vars->bytes_read] = 0;
	//send(vars->events[fdToCheck].data.fd, vars->buffer, vars->bytes_read, 0);
	printf("message: %s\n", vars->buffer);
	send(vars->events[fdToCheck].data.fd, html, strlen(html), 0);
	//sleep(4);
}

int checkClientsSockets(struct vars *vars, int fdToCheck)
{
	int check = 0;
	bool read = somethingToRead(vars, fdToCheck, &check);
	if (read == false && check == -1)
		return (-1);
	else if (read == false && check == 0)
		closeConnection(vars, fdToCheck);
	else if (read == true)
		echoMessage(vars, fdToCheck);;
	return (0);
}

int checkEpoll(struct vars *vars)
{
	int n = 0;

	while (n < vars->nfds)
	{
		if (incomingNewConnection(vars, n) == true)
		{
			if (acceptNewConnection(vars) == -1)
				return (-1);
			if (addNewSocketToEpoll(vars) == -1)
				return (-1);
		}
		else 
		{
			if ((checkClientsSockets(vars, n) == -1))
				return (-1);
		}	
		n++;
	}
	return (0);
}

/*void signalHandler(int fd)
{
	(void) fd;
	exit(0);
}*/

int acceptIncomingConnection(struct vars *vars)
{
	while (1)
	{
		if (waitForConnection(vars) == -1)
			break ;
		if (checkEpoll(vars) == -1)
			break ;
		/*if (signal(SIGINT, signalHandler) == SIG_ERR)
		{
			perror("Error al establecer el manejadr SIGINT");
			break ;
		}*/
	}
	return (-1);
}

int main(void)
{
	vars vars;

	if (createMasterSocket(&vars) == -1)
		return (-1);
	if(confSocket(&vars) == -1)
		return (-1);
	setTypeOfSocketCreated(&vars);
	if (associateSocketWithPortIP(&vars))
		return (-1);
	if (listenForRequests(&vars) == -1)
		return (-1);
	if (instanceEpoll(&vars) == -1)
		return (-1);
	if (addMasterSocketToEpoll(&vars) == -1)
		return (-1);
	printf("Waiting for connections ...\n");
	if (acceptIncomingConnection(&vars))
		return (-1);
	close(vars.master_socket);
	close(vars.epoll_fd);
	return (0);	
}
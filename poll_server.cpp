/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:49:40 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/12 12:48:51 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//int poll(struct pollfd *fds, nfds_t nfds, int timeout)

#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 10

struct vars
{
	int master_socket;
	int new_socket;
	int client_socket[MAX_CLIENTS];
	struct sockaddr_in address;
	socklen_t addrlen;
	char buffer[1025];
	struct pollfd fds[MAX_CLIENTS + 1];
	int nfds = 1;
	int valread;
};

void initClientSockets(struct vars *vars)
{
	int i = 1;

	while (i < MAX_CLIENTS)
	{
		vars->fds[i].fd = -1;
		vars->fds[i].events = 0;
		vars->fds[i].revents = 0;
		i++;
	}
}

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

void setTypeOfSocketCreated(struct vars *vars)
{
	vars->address.sin_family = AF_INET;
	vars->address.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	vars->address.sin_port = htons(PORT);
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

int ListenForRequests(struct vars *vars)
{
	if (listen(vars->master_socket, 3) == -1)
	{
		perror("listen\n");
		close(vars->master_socket);
		return (-1);
	}
	return (0);
}

void initPollFd(struct vars *vars)
{
	vars->fds[0].fd = vars->master_socket;
	vars->fds[0].events = POLLIN;
}

int waitForActivity(struct vars *vars)
{
	int activity = poll(vars->fds, vars->nfds, -1);
	if (activity < 0)
	{
		perror("poll error\n");
		return (-1);
	}
	return (0);
}

bool MasterSocketReady(struct vars *vars)
{
	if (vars->fds[0].revents & POLLIN)
		return (true);
	else
		return (false);
}

int addNewSocket(struct vars *vars)
{
	vars->new_socket = accept(vars->master_socket, (struct sockaddr *)&vars->address, &vars->addrlen);
	if (vars->new_socket < 0)
		return (-1);
	printf("New connection, socket fd %d, ip is : %s, port : %d\n", vars->new_socket, inet_ntoa(vars->address.sin_addr), ntohs(vars->address.sin_port));
	return (0);
}

void addSocketPollArray(struct vars *vars)
{
	int i = 1;
	
	while (i < MAX_CLIENTS)
	{
		if (vars->fds[i].fd == -1)
		{
			vars->fds[i].fd = vars->new_socket;
			vars->fds[i].events = POLLIN;
			vars->nfds = (i >= vars->nfds) ? (i + 1) : vars->nfds;
			break ;
		}
		i++;
	}
}

bool anythingToRead(int fd, struct vars *vars)
{	
	vars->valread = read(fd, vars->buffer, 1024);
	if (vars->valread == 0)
		return (false);
	else
		return (true);
}

void echoMessage(int fd, struct vars *vars)
{
	vars->buffer[vars->valread] =0;
	send(fd, vars->buffer, strlen(vars->buffer), 0);
}

void closeConection(int *fd, struct vars *vars)
{
	getpeername(*fd, (struct sockaddr *)&vars->address, (socklen_t *)&vars->addrlen);
	printf("Host disconnected. ip %s, port %d\n", inet_ntoa(vars->address.sin_addr), ntohs(vars->address.sin_port));
	close(*fd);
	*fd = -1;
}

void CheckClientSockets(struct vars *vars)
{
	int i = 1;
	
	while (i < vars->nfds)
	{
		if (vars->fds[i].fd > 0 && (vars->fds[i].revents & POLLIN))
		{
			if (anythingToRead(vars->fds[i].fd, vars) == true)
				echoMessage(vars->fds[i].fd, vars);
			else
				closeConection(&vars->fds[i].fd, vars);
		}
		i++;
	}
}

int acceptIncomingConnection(struct vars *vars)
{
	while (1)
	{
		if (waitForActivity(vars) == -1)
			break ;
		if (MasterSocketReady(vars) == true)
		{
			if (addNewSocket(vars) == -1)
				break ;
			addSocketPollArray(vars);
		}
		CheckClientSockets(vars);
	}
	return (-1);
}

int main(void)
{
	vars vars;
	
	initClientSockets(&vars);
	if (createMasterSocket(&vars) == -1)
		return (-1);
	setTypeOfSocketCreated(&vars);
	if (associateSocketWithPortIP(&vars) == -1)
		return (-1);
	if (ListenForRequests(&vars) == -1)
		return (-1);
	vars.addrlen = sizeof(vars.address);
	printf("Waiting for connections ...\n");
	initPollFd(&vars); //inicializa laa estructura pollfd para el socket maestro
	if (acceptIncomingConnection(&vars) == -1)
		return (-1);
	return (0);
}

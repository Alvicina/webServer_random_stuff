/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:18:46 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/11 10:23:36 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//int vars(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>

#define PORT 8080
#define MAX_CLIENTS 10

struct vars
{
	int master_socket;
	int addrlen;
	int new_socket;
	int client_socket[MAX_CLIENTS];
	int max_clients = MAX_CLIENTS;
	int activity;
	int i;
	int valread;
	int sd;
	int max_sd;
	char buffer[1025];
	struct sockaddr_in address;
	char const *message = "ECHO Daemon v1.0 \r\n";
};


void initClientSockets(struct vars *vars)
{
	int i = 0;

	while (i < MAX_CLIENTS)
	{
		vars->client_socket[i] = 0;
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
	vars->address.sin_addr.s_addr = inet_addr("127.0.0.1");
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

void clearSocketSet(fd_set *readfds)
{
	FD_ZERO(readfds);
}

void addMasterSocket(struct vars *vars, fd_set *readfds)
{
	FD_SET(vars->master_socket, readfds);
	vars->max_sd = vars->master_socket;	
}

void addClientSocket(struct vars *vars, fd_set *readfds)
{	
	int i = 0;
	
	while (i < MAX_CLIENTS)
	{
		vars->sd = vars->client_socket[i];
		if (vars->sd > 0)
			FD_SET(vars->sd, readfds);
		// Highest file descriptor number, need it for the vars function
		if (vars->sd > vars->max_sd)
			vars->max_sd = vars->sd;
		i++;	
	}
}

int waitForActivity(struct vars *vars, fd_set *readfds)
{
	vars->activity = select(vars->max_sd + 1, readfds, NULL, NULL, NULL);
	if (vars->activity < 0 && errno != EINTR)
	{
		perror("Select error\n");
		return (-1);
	} 
	return (0);
}

void sendMessage(int socket, char const *message)
{
	if (send(socket, message, strlen(message), 0) == -1)
	{
		perror("send\n");
		return ;
	}
	//printf("message: %s\n", message);
	printf("Message sent successfully\n");	
}

int createNewSocket(struct vars *vars)
{
	if ((vars->new_socket = accept(vars->master_socket, (struct sockaddr *)&vars->address, (socklen_t *)&vars->addrlen)) < 0)
	{
		perror("accept\n");
		return (-1);
	}
	return (0);
}

void addSocketToArray(struct vars *vars)
{
	int i = 0;

	while (i < MAX_CLIENTS)
	{
		if (vars->client_socket[i] == 0)
		{
			vars->client_socket[i] = vars->new_socket;
			printf("Adding to list of sockets as %d\n", i);
			break ;
		}
		i++;
	}
}

int checkMasterSocket(struct vars *vars, fd_set *readfds)
{
	if (FD_ISSET(vars->master_socket, readfds))
	{
		if (createNewSocket(vars) == -1)
			return (-1);
		printf("New connection, socket fd is %d, ip is: %s, port: %d\n", vars->new_socket, inet_ntoa(vars->address.sin_addr), ntohs(vars->address.sin_port));
		sendMessage(vars->new_socket, vars->message);
		addSocketToArray(vars);
	}
	return (0);
}

bool checkIfSocketReady(struct vars *vars, fd_set *readfds)
{
	if (FD_ISSET(vars->sd, readfds))
		return (true);
	return (false);
}

void closeSocket(struct vars *vars, int i)
{
	getpeername(vars->sd, (struct sockaddr *)&vars->address, (socklen_t *)&vars->addrlen);
	printf("Host disconnected, ip %s, port %d\n", inet_ntoa(vars->address.sin_addr), ntohs(vars->address.sin_port));
	close(vars->sd);
	vars->client_socket[i] = 0;
}

void echoMessage(struct vars *vars)
{
	vars->buffer[vars->valread] = 0;
	sendMessage(vars->sd, vars->buffer);
}

void readSocket(struct vars *vars, int i)
{
	vars->valread = read(vars->sd, vars->buffer, 1024);
	if (vars->valread == 0)
		closeSocket(vars, i); //se han desconectado
	else
		echoMessage(vars); //devuelve el mensaje recibido
}

void checkClientSocket(struct vars *vars, fd_set *readfds)
{
	int i = 0;

	while (i < MAX_CLIENTS)
	{
		vars->sd = vars->client_socket[i];
		if (checkIfSocketReady(vars, readfds) == true)
			readSocket(vars, i);
		i++;
	}
}

int acceptIncomingConnection(struct vars *vars, fd_set *readfds)
{
	while (1)
	{
		clearSocketSet(readfds);
		addMasterSocket(vars, readfds);
		addClientSocket(vars, readfds);
		if (waitForActivity(vars, readfds) == -1)
			break ;
		checkMasterSocket(vars, readfds);
		checkClientSocket(vars, readfds);
	}
	return (-1);
}

int main(void)
{
	struct vars vars;
	
	//set of socket descriptors
	fd_set readfds;
	initClientSockets(&vars);
	if (createMasterSocket(&vars) == -1)
		return (-1);
	setTypeOfSocketCreated(&vars);
	if (associateSocketWithPortIP(&vars) == -1)
		return (-1);
	if (ListenForRequests(&vars) == -1)
		return (-1);
	vars.addrlen = sizeof(vars.address);
	//accept the incoming connection
	printf("Esperando conexiones....\n");
	if (acceptIncomingConnection(&vars, &readfds) == -1)
		return (-1);
	return (0);
}

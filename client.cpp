/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:10:43 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/12 17:51:03 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//int socket(int __domain, int __type, int __protocol)
//in_addr_t inet_addr(const char *__cp)
//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
//ssize_t send(int __fd, const void *__buf, size_t __n, int __flags)
//ssize_t recv(int __fd, void *__buf, size_t __n, int __flags)

int	main(void)
{
	int socketfd;
	struct sockaddr_in server_addr;

	//creamos socket
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1)
	{
		perror("socket");
		return (-1);
	}
	//inicializamos la estructura sockaddr_in
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//conectamos al servidor
	if (connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(socketfd);
		return (-1);
	}
	printf("conectando al servidor...\n");
	sleep(5);
	//enviar mensaje al seridor
	char	const *msg = "mensaje del cliente";
	send(socketfd, msg, strlen(msg), 0); 
	
	//recibir respuesta del servidor
	char	buffer[1024];
	int 	bytes_received = recv(socketfd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received > 0)
	{
		buffer[bytes_received] = 0;
		printf("Cliente: %s\n", buffer);
	}
	close(socketfd);
	//read(0, buffer, 1);
	return (0);
}
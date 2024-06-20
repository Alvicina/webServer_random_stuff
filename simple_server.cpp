/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 11:46:51 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/07 13:01:44 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


//int bind(int __fd, const sockaddr *__addr, socklen_t __len)
//int listen(int __fd, int __n)
//int accept(int __fd, sockaddr *__restrict__ __addr, socklen_t *__restrict__ __addr_len)

int main(void)
{
	int sockfd;
	int new_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t addr_size;
	
	//crea un socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket");
		return (-1);
	}
	//inicializa la estructura sockaddr_in del servidor
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//asocia el socket con el puntero y la direccion IP del servidor
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(sockfd);
		return (-1);
	}
	//escucha conexiones entrantes
	if (listen(sockfd, 5) == -1)
	{
		perror("listen");
		close(sockfd);
		return (-1);
	}
	printf("Esperando conexiones...\n");
	//aceptar una conexion entrante
	addr_size = sizeof(client_addr);
	new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
	if (new_sock == -1)
	{
		perror("accept");
		close(sockfd);
		return (-1);
	}
	printf("Cliente conectado\n");
	//recibir un mensaje del cliente
	char buffer[1024];
	int bytes_received = recv(new_sock, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received > 0)
	{
		buffer[bytes_received] = 0;
		printf("Servidor: %s\n", buffer);
	}
	//enviar una respuesta al cliente
	char const *msg = "mensaje del servidor";
	send(new_sock, msg, strlen(msg), 0);
	close(new_sock);
	close(sockfd);
	return (0);
}
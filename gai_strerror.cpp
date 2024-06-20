/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gai_strerror.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:43:09 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/06 13:45:38 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

//const char *gai_strerror(int errcode);

int	main(void)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int				status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	status = getaddrinfo("pepe", "80", &hints, &res); //hay que liberar memoria dinamica
	printf("%d\n", status);
	if (status != 0)
	{
		printf("Error al getaddrinfo: %s\n", gai_strerror(status));
		return (1);
	}
	printf("hola");
	freeaddrinfo(res);
	return (0);
}

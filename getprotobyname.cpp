/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getprotobyname.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:25:47 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/13 12:52:47 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>

 struct protoent **getProtocolName(char const *protocolToGet)
{
	struct protoent **proto = (protoent **)malloc(sizeof(protoent));
	
	*proto = getprotobyname(protocolToGet);
	if (*proto == NULL)
	{
		perror("getprotobyname\n");
		return (NULL);
	}
	else
		return (proto);
}

void printAliases(protoent *proto)
{
	if (proto->p_aliases != NULL)
	{
		int i = 0;
		printf("Alias del protocolo\n");
		while (proto->p_aliases[i] != NULL)
		{
			printf("%s\n", proto->p_aliases[i]);
			i++;
		}
	}
	else
		printf("No hay alias para el protocolo\n");
}

void printProtocolInfo(protoent *proto)
{
	printf("Nombre del protocolo: %s\n", proto->p_name);
	printf("Número del protocolo; %d\n", proto->p_proto);
	printAliases(proto);
}

int main(void)
{
	char const *protocolToGet = "tcp";
	
	struct protoent **proto = getProtocolName(protocolToGet);
	if (proto == NULL)
		return (-1);
	printProtocolInfo(*proto);
	free(proto);
	//proto = NULL;
	return (0);
}
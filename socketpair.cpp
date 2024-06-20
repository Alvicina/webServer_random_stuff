/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketpair.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:22:06 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/06 17:50:57 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//int socketpair(int domain, int type, int protocol, int sv[2]);

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int	main(void)
{
	int		sv[2];
	char	buf[100];
	ssize_t	nbytes;
	
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
	{
		perror("sockerpair");
		return (-1);		
	}
	//pipe(sv);
	if (fork() == 0)
	{
		close(sv[0]);
		char const *msg = "Hola Alejandro";
		write(sv[1], msg, strlen(msg));
		close(sv[1]);
		return (0);
	}
	close(sv[1]);
	nbytes = read(sv[0], buf, sizeof(buf));
	close (sv[0]);
	if (nbytes > 0)
	{
		buf[nbytes] = 0;
		printf("%s\n", buf);
		return (0);
	}
	else
	{
		perror("read");
		return (-1);
	}	
}
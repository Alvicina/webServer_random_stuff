/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcntl.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:47:18 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/13 17:18:25 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int getFD(char const *file)
{
	int fd = open(file, O_RDWR);
	if (fd == -1)
	{
		perror("open\n");
		return (-1);
	}
	return (0);
}

int getFlags(int *fd)
{
	int flags = fcntl(*fd, F_GETFL);
	if (flags == -1)
	{
		perror("fcntl\n");
		close(*fd);
		return (-1);
	}
	return (flags);
}

void printFlags(int flags)
{
	printf("flags: %d\n", flags);
}

int setFlags(int *fd, int *flags)
{
	*flags |= FD_CLOEXEC;
	if (fcntl(*fd, F_SETFD, *flags) == -1)
	{
		perror("fcntl\n");
		close(*fd);
		return (-1);
	}
	printf("flags set\n");
	return (0);
}

int main(void)
{
	char const *file = "example.txt";
	int fd = getFD(file);
	if (fd == -1)
		return (-1);
	int flags = getFlags(&fd);
	if (flags == -1)
		return (-1);
	printFlags(flags);
	setFlags(&fd, &flags);
	close(fd);
	return (0);
}
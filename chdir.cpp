/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:54:27 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/13 12:06:26 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int changeDirectory(void)
{
	if (chdir("/tmp") == -1)
	{
		perror("chdir\n");
		return (-1);
	}
	return (0);
}

void printCwd(char *cwd)
{
	printf("cwd: %s\n", cwd);
	free(cwd);
}

int getCurrentWorkingDirectory()
{
	char *cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd\n");
		return (-1);
	}
	printCwd(cwd);
	return (0);
	
}

int main(void)
{
	if (changeDirectory() == -1)
		return (-1);
	if (getCurrentWorkingDirectory() == -1)
		return (-1);
	return (0);
}
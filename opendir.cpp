/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opendir.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:46:07 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/14 11:06:30 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int openDirectory(char const **directoryPath, DIR **dir)
{
	*dir = opendir(*directoryPath);
	if (*dir == NULL)
	{
		perror("opendir\n");
		return (-1);
	}
	return (0);
}

void printEntry(struct dirent **entry)
{
	printf("%s\n", (*entry)->d_name);
}

int readDirectory(DIR **dir, int *error)
{
	struct dirent *entry;
	
	entry = readdir(*dir);
	if (entry != NULL)
	{
		printEntry(&entry);
		return (0);
	}
	else
	{
		*error = -1;
		return (-1);
	}
}

int main(void)
{
	char const		*directoryPath = ".";;
	DIR 			*dir;
	int 			error;

	if (openDirectory(&directoryPath, &dir) == -1)
		return (-1);
	while (readDirectory(&dir, &error) != -1)
		continue ;
	closedir(dir);
	if (error == -1)
		return (-1);
	return (0);
}
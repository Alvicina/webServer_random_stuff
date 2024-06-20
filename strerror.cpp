/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strerror.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:05:55 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/06 13:45:57 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("non_existant_file.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("errno: %d\n", errno);
		//char *error = strerror(errno);
		printf("error: %s\n", strerror(errno));
	}
	return (EXIT_SUCCESS);
}
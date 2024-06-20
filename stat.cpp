/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:31:14 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/13 17:37:43 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int getStats(char const *path, struct stat *sb)
{
	if (stat(path, sb) == -1)
	{
		perror("stat\n");
		return (1);
	}
	return (0);
}

void printStats(char const *path, struct stat *sb)
{
	printf("Archivo: %s\n", path);
    printf("ID del dispositivo: %ld\n", (long) sb->st_dev);
    printf("Número de inodo: %ld\n", (long) sb->st_ino);
    printf("Tipo y permisos: %o\n", (unsigned int) sb->st_mode);
    printf("Número de enlaces duros: %ld\n", (long) sb->st_nlink);
    printf("ID del propietario: %d\n", (int) sb->st_uid);
    printf("ID del grupo: %d\n", (int) sb->st_gid);
    printf("Tamaño: %lld bytes\n", (long long) sb->st_size);
    printf("Tamaño de bloque preferido: %ld bytes\n", (long) sb->st_blksize);
    printf("Número de bloques: %lld\n", (long long) sb->st_blocks);
    printf("Último acceso: %ld\n", (long) sb->st_atime);
    printf("Última modificación: %ld\n", (long) sb->st_mtime);
    printf("Último cambio de estado: %ld\n", (long) sb->st_ctime);
}

int main(void)
{
	struct stat sb;
	char const *path = "example.txt";

	if (getStats(path, &sb) == -1)
		return (-1);
	printStats(path, &sb);
	return (0);
}
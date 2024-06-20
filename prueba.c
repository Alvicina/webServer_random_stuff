/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:59:19 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/13 13:01:10 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char *doMalloc()
{
	char *s = malloc(6);
	if (s == NULL)
		return (NULL);
	else 
		return (s);
}

int main(void)
{
	char *string = doMalloc();
	free(string);
	return (0);
}
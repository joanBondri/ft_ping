/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 11:04:50 by ncolomer          #+#    #+#             */
/*   Updated: 2019/10/07 17:25:34 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int
	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z'));
}

int
	ft_isalphasup(int c)
{
	return ((c >= 'A' && c <= 'Z'));
}


int
	ft_ishex(int c)
{
	return ((c >= 'A' && c <= 'F') ||
			(c >= 'a' && c <= 'f') ||
			ft_isdigit(c));
}

#include "libft.h"

int ft_maxlen(char* s1, char* s2)
{
	int l1 = ft_strlen(s1);
	int l2 = ft_strlen(s2);
	if (l1 > l2)
		return l1;
	return l2;
}
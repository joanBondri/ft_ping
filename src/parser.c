#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void helpMessageError(void)
{
	fprintf(stderr, "Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
	return ;
}


uint checkNumOrThrow(char* str)
{
	uint res = 0;
	int diz = 1;
	int i = 0;
	for (; ft_isdigit(str[i]); i++)
	{
		res = res * diz + (str[i] - '0');
		diz = diz * 10;
		if (res >= 3445453453)
			return (-1);
	}
	if (str[i])
	{
		return (-1);
	}
	return res;
}

int	manageLoad(char* str, char* str2)
{
	int 	res;
	char*	test;

	if (str && str[1] != '\0')
		test = str + 1;
	else
		test = str2;
	if (test == NULL)
	{
		fprintf(stderr, "ping: option requires an argument -- 'l'\n");
		helpMessageError();
		return (-1);
	}
	res = checkNumOrThrow(test);
	if (res == -1)
	{
		fprintf(stderr, "ping: invalid preload value (%s)\n", test);
	}
	return res;
}

int optPing(parsedData_t* setup, char* str, char *str2)
{
	int len = ft_strlen(str);
	if (!ft_strncmp(str, "-", ft_strlen(str)))
	{
		ft_lstadd_back(&(setup->domains), ft_lstnew(str));
		return (0);
	}
	for (int i = 1; i < len; i++)
	{
		if (str[i] == 'v')
			setup->verbose = true;
		else if (str[i] == 'f')
			setup->flood = true;
		else if (str[i] == 'l')
		{
			setup->load = manageLoad(str + i, str2);
			if (setup->load == -1)
			{
				return (-1);
			}
			return (0);
		}
		else if (str[i] == '?')
		{
			setup->help = HELP;
			break ;
		}
		else
		{
			fprintf(stderr, "ft_ping: invalid option -- '%c'\n", str[i]);
			helpMessageError();
			return (-1);
		}
	}
	return (0);
}

int optionsPing(parsedData_t* setup, char* str, char* str2)
{
	if (!ft_strncmp(str, "--help", ft_maxlen(str, "--help")))
		setup->help = HELP;
	else if (!ft_strncmp(str, "--usage", ft_maxlen(str, "--usage")))
		setup->help = USAGE;
	else if (!ft_strncmp(str, "--verbose", ft_maxlen(str, "--verbose")))
		setup->verbose = true;
	else if (!ft_strncmp(str, "--flood", ft_maxlen(str, "--flood")))
		setup->flood = true;
	else if (!ft_strncmp(str, "--load", ft_maxlen(str, "--load")))
	{
		setup->load = manageLoad(NULL, str2);
		if (setup->load == -1)
		{
			return (-1);
		}
	}
	else if (ft_strncmp(str, "--", ft_strlen(str)))
	{
		fprintf(stderr, "ft_ping: unrecognized option '%s'\n", str);
		helpMessageError();
		return (-1);
	}
	return (0);
}

void parseInputPing(char **argv, parsedData_t* setup)
{
	int res = 0;
	if (argv[1] == NULL)
	{
		fprintf(stderr, "ft_ping: missing host operand\n");
		helpMessageError();
		exit(64);

	}
	for (char* str = *++argv; str != NULL; str = *++argv)
	{
		if (!ft_strncmp(str, "--", 2))
		{
			if (optionsPing(setup, str, argv[1]) == -1)
			{
				ft_lstclear(&(setup->domains), nullfunc);
				exit(64);
			}
		}
		else if (!(res = ft_strncmp(str, "-", 1)))
		{
			if (optPing(setup, str, argv[1]) == -1)
			{
				ft_lstclear(&(setup->domains), nullfunc);
				exit(64);
			}
		}
		else
			ft_lstadd_back(&(setup->domains), ft_lstnew(str));
		if (setup->help != NONE)
			break ;
	}
}

void nullfunc(void* data)
{
	(void)data;
}
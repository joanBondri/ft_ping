#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void helpMessageError(void)
{
	fprintf(stderr, "Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
	return ;
}

int optionsPing(parsedData_t* setup, char* str)
{
	if (!ft_strncmp(str, "--help", ft_maxlen(str, "--help")))
		setup->help = HELP;
	else if (!ft_strncmp(str, "--usage", ft_maxlen(str, "--usage")))
		setup->help = USAGE;
	else if (!ft_strncmp(str, "--verbose", ft_maxlen(str, "--verbose")))
		setup->verbose = true;
	else if (ft_strncmp(str, "--", ft_strlen(str)))
	{
		fprintf(stderr, "ft_ping: unrecognized option '%s'\n", str);
		helpMessageError();
		return (-1);
	}
	return (0);
}

int optPing(parsedData_t* setup, char* str)
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
			if (optionsPing(setup, str) == -1)
			{
				ft_lstclear(&(setup->domains), nullfunc);
				exit(64);
			}
		}
		else if (!(res = ft_strncmp(str, "-", 1)))
		{
			if (optPing(setup, str) == -1)
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
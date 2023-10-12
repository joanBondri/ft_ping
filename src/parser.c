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
		else if (str[i] == 'n')
			setup->numeric = true;
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
	if (!ft_strncmp(str, "--help", sizeof("--help")))
		setup->help = HELP;
	else if (!ft_strncmp(str, "--usage", sizeof("--usage")))
		setup->help = USAGE;
	else if (!ft_strncmp(str, "--verbose", sizeof("--verbose")))
		setup->verbose = true;
	else if (!ft_strncmp(str, "--numeric", sizeof("--numeric")))
		setup->numeric = true;
	else if (!ft_strncmp(str, "--flood", sizeof("--flood")))
		setup->flood = true;
	else if (!ft_strncmp(str, "--preload", sizeof("--preload")))
	{
		if(!ft_strncmp(str, "--preload=", sizeof("--preload=")))
			setup->load = manageLoad(str + sizeof("--preload="), str2);
		else
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

char* checkNumericorThrow(char* str)
{
	int len = ft_strlen(str);
	for(int i = 0; i < len; i++)
	{
		if(!ft_isdigit(str[i]))
			return str + i;
	}
	return NULL;
}

char* checkHexorThrow(char* str)
{
	int len = ft_strlen(str);
	for(int i = 0; i < len; i++)
	{
		if(!ft_ishex(str[i]))
			return str + i;
		if(i > 6)
			return str;
	}
	return NULL;
}

long manageoptionsnumeric(char* str, char* value, char* error)
{
	char* focus;
	if(!str || str[0] == '\0')
	{
		focus = value;
	}
	else
		focus = str;
	error = checkNumericorThrow(focus);
	if(error)
		return -2;
	str = focus;
	return (long)ft_atol(focus);
}

long manageoptionshex(char* str, char* value, char* error)
{
	char* focus;
	if(!str || str[0] == '\0')
	{
		focus = value;
	}
	else
		focus = str;
	error = checkHexorThrow(focus);
	if(error)
		return -2;
	str = focus;
	return (long)ft_atohex(focus);
}

int optPing2(parsedData_t* setup, char* str, char* value, int* i)
{
	int j = 0;
	char* error = NULL;
	long res;
	char* strbis = NULL;

	if(str[1] == '\n')
	{
		dprintf(STDERR_FILENO, "%s: invalid option -- '%c'\n", "ft_ping", '-');
	}
	if(!ft_strncmp(str, "--ttl", sizeof("--ttl")))
	{
		char* focus;
		if(!ft_strncmp(str, "--ttl=", sizeof("--ttl=")))
			focus = str + 6;
		else
			focus = value;
		if ((res = manageoptionsnumeric(strbis, focus, error)) == -2)
		{
			dprintf(STDOUT_FILENO, "ping: invalid value (`%s' near `%s')\n", str, error);
			return -1;
		}
		if (res < 0 || res > 128)
		{
			dprintf(STDOUT_FILENO, "ping: option value too big: %s\n", str);
		}
		setup->ttl = res;
		if(value == strbis)
			(*i)++;
		return 0;
	}
	while(str[++j] != '\0')
	{
		if(str[j] == FLOOD)
		{
			setup->flood = true;
		}
		if(str[j] == RESTRICTEDROUTING)
		{
			setup->restrictedRouting = true;
		}
		if(str[j] == NUMERIC)
		{
			setup->numeric = true;
		}
		else if(str[j] == LOAD)
		{

			strbis = str + j + 1;
			if ((res = manageoptionsnumeric(strbis, value, error)) < 0 || res > 100000)
			{
				dprintf(STDOUT_FILENO, "ping: invalid preload value (%s)\n", strbis);
				return -1;
			}
			setup->load = (uint)res;
			if(value == strbis)
				(*i)++;
			return 0;
		}
		else if(str[j] == SIZEDATA)
		{
			strbis = str + j + 1;
			if ((res = manageoptionsnumeric(strbis, value, error)) == -2)
			{
				dprintf(STDOUT_FILENO, "ping: invalid value (`%s' near `%s')\n", str, error);
				return -1;
			}
			if (res < 0 || res > 100000)
			{
				dprintf(STDOUT_FILENO, "ping: option value too big: %s\n", str);
			}
			setup->sizeData = (size_t)res;
			if(value == strbis)
				(*i)++;
			return 0;
		}
		else if(str[j] == WAITING)
		{
			strbis = str + j + 1;
			if ((res = manageoptionsnumeric(strbis, value, error)) == -2)
			{
				dprintf(STDOUT_FILENO, "ping: invalid value (`%s' near `%s')\n", str, error);
				return -1;
			}
			if (res < 0 || res > 100000)
			{
				dprintf(STDOUT_FILENO, "ping: option value too big: %s\n", str);
			}
			setup->waiting = res;
			if(value == strbis)
				(*i)++;
			return 0;
		}
		else if(str[j] == WAITINGW)
		{
			strbis = str + j + 1;
			if ((res = manageoptionsnumeric(strbis, value, error)) == -2)
			{
				dprintf(STDOUT_FILENO, "ping: invalid value (`%s' near `%s')\n", str, error);
				return -1;
			}
			if (res < 0 || res > 100000)
			{
				dprintf(STDOUT_FILENO, "ping: option value too big: %s\n", str);
			}
			if(value == strbis)
				(*i)++;
			return 0;
		}
		else if(str[j] == PAD)
		{
			strbis = str + j + 1;
			if ((res = manageoptionshex(strbis, value, error)) == -2)
			{
				dprintf(STDOUT_FILENO, "ping: error in pattern near %s\n", error);
				return -1;
			}
			setup->pad = (u_int16_t)res;
			if(value == strbis)
				(*i)++;
			return 0;
		}
		else
		{
			fprintf(stderr, "ft_ping: invalid option -- '%c'\n", str[j]);
			helpMessageError();
			return (-1);
		}
	}
	return 0;
}

void parseInputPing2(int argc, char **argv, parsedData_t* setup)
{
	int res = 0;
	if (argv[1] == NULL)
	{
		fprintf(stderr, "ft_ping: missing host operand\n");
		helpMessageError();
		exit(64);

	}
	for (int i = 1; i != argc; i++)
	{
		printf("focus = %s\n", argv[i]);
		printf("value = %s\n", argv[i + 1]);
		if (!(res = ft_strncmp(argv[i], "-", 1)))
		{
			if (optPing2(setup, argv[i], argv[i + 1], &i) == -1)
			{
				ft_lstclear(&(setup->domains), nullfunc);
				exit(64);
			}
		}
		else
			ft_lstadd_back(&(setup->domains), ft_lstnew((void*)argv[i]));
		if (setup->help != NONE)
			break ;
	}
}


void nullfunc(void* data)
{
	(void)data;
}
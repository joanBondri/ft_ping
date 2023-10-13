#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void helpMessageError(void)
{
	fprintf(stderr, "Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
	return ;
}

void optRequireArg(char* c)
{
	fprintf(stderr, "ping: option requires an argument -- '%s'\n", c);
	return ;
}

void nullfunc(void* data)
{
	(void)data;
}

char*	ttlExtracted(char* actual, char *value, int* i)
{
	if (!ft_strncmp(actual, "--ttl=", 6))
		return actual + 6;
	(*i)++;
	return value;
}

char*	optExtracted(char* actual, char *value, int* i)
{
	if (actual[1])
		return actual + 1;
	(*i)++;
	return value;
}
int		checkValidityLoad(char *brut)
{
	if (!brut)
	{
		optRequireArg("l");
		return (64);
	}
	int len = ft_strlen(brut);
	int i;
	for (i = 0; i < len; i++)
	{
		if (!ft_isdigit(brut[i]))
			break ;
	}
	if (brut[i] || ft_atol(brut) > INT_MAX)
	{
		fprintf(stderr, "ft_ping: invalid preload value (%s)\n", brut);
		return (1);
	}
	return (0);
}

int		checkValidityPad(char *brut)
{
	if (!brut)
	{
		optRequireArg("p");
		return (64);
	}
	int len = ft_strlen(brut);
	int i;
	for (i = 0; i < len; i++)
	{
		if (!ft_ishex(brut[i]))
			break ;
	}
	if (brut[i])
	{
		fprintf(stderr, "ping: error in pattern near %s\n", brut + i);
		return (1);
	}
	return (0);
}

int	extractPad(char *s, long* value)
{
	*value = ft_atohex(s);
	return (0);
}

int		checkValidityNumber(char* brut)
{
	int len = ft_strlen(brut);
	int i;
	long	res;
	for (i = 0; i < len; i++)
	{
		if(i == 0 && brut[0] == '-')
			continue ;
		if (!ft_isdigit(brut[i]))
			break ;
	}
	if (brut[i])
	{
		fprintf(stderr, "ping: invalid value (`%s' near `%s')\n", brut, brut + i);
		return (1);
	}
	res = ft_atol(brut);
	if (res > INT_MAX || res < 0)
	{
		fprintf(stderr, "ping: option value too big: %s\n", brut);
		return (1);
	}
	return (0);
}

int		extractLoad(char* brut, long* value)
{
	(*value) = ft_atol(brut);
	return (0);
}

int		extractValue(char* brut, long extrem, long* value)
{
	(*value) = ft_atol(brut);
	if (*value > extrem || *value < 0)
	{
		fprintf(stderr, "ping: option value too big: %s\n", brut);
		return (1);
	}
	return (0);
}

int		parseFlag(char* actual, char *value, int* i, parsedData_t* setup)
{
	char*	dataExtracted;
	int		res;
	long	finalValue;

	if (!ft_strncmp(actual, "--ttl", 5))
	{
		dataExtracted = ttlExtracted(actual, value, i);
		if (!dataExtracted)
		{
			optRequireArg("--ttl");
			return (64);
		}
		if((res = checkValidityNumber(dataExtracted)) || (res = extractValue(dataExtracted, INT_MAX, &finalValue)))
			return res;
		setup->ttl = finalValue;
		return (0);
	}
	if (!ft_strncmp(actual, "--help", 5))
		setup->help = HELP;
	if (!ft_strncmp(actual, "--usage", 5))
		setup->help = USAGE;
	for(int j = 1; actual[j]; j++)
	{
		if(setup->help != NONE)
			return (0);
		switch (actual[j])
		{
			case FLOOD:
				setup->flood = true;
				break;
			case '?':
				setup->help = HELP;
				return (0);
				break;
			case VERBOSE:
				setup->verbose = true;
				break;
			case NUMERIC:
				setup->numeric = true;
				break;
			case RESTRICTEDROUTING:
				setup->restrictedRouting = true;
				break;
			case SIZEDATA:
				dataExtracted = optExtracted(actual + j, value, i);
				if (!dataExtracted)
				{
					optRequireArg("s");
					return (64);
				}
				if((res = checkValidityNumber(dataExtracted)) || (res = extractValue(dataExtracted, SHRT_MAX, &finalValue)))
					return res;
				setup->sizeData = finalValue;
				return (0);
				break ;
			case LOAD:
				dataExtracted = optExtracted(actual + j, value, i);
				if((res = checkValidityLoad(dataExtracted)) || (res = extractLoad(dataExtracted, &finalValue)))
					return res;
				setup->load = finalValue;
				return (0);
				break ;
				case PAD:
					dataExtracted = optExtracted(actual + j, value, i);
					if((res = checkValidityPad(dataExtracted)) || (res = extractPad(dataExtracted, &finalValue)))
						return res;
					printf("final = %ld\n", finalValue);
					setup->pad = finalValue;
					return (0);
					break ;
			default :
				dataExtracted = optExtracted(actual + j, value, i);
				if (!dataExtracted)
				{
					optRequireArg("s");
					return (64);
				}
				if((res = checkValidityNumber(dataExtracted)) || (res = extractValue(dataExtracted, INT_MAX, &finalValue)))
					return res;
				switch (actual[j])
				{
				case WAITING:
					setup->waiting = finalValue;
					return (0);
					break ;
				case WAITINGW:
					setup->waitingW = finalValue;
					return (0);
					break ;
				default :
					fprintf(stderr, "ping: invalid option -- '%c'\n", actual[j]);
					helpMessageError();
					return (64);
					break ;
				}
		}
	}
	return (0);
}

void parseInputPing2(int argc, char **argv, parsedData_t* setup)
{
	int res = 0;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if ((res = parseFlag(argv[i], argv[i + 1], &i, setup)))
			{
				ft_lstclear(&(setup->domains), nullfunc);
				exit(res);
			}
		}
		else
			ft_lstadd_back(&(setup->domains), ft_lstnew(argv[i]));
		if (setup->help != NONE)
			break ;
	}
	if (!(setup->domains))
	{
		fprintf(stderr, "ft_ping: missing host operand\n");
		helpMessageError();
		exit(64);
	}
}
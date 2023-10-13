#ifndef PARSER_H
# define PARSER_H
# include <stdbool.h>
# include "libft.h"
# include <limits.h>
# define VERBOSE 'v'
# define FLOOD 'f'
# define LOAD 'l'
# define NUMERIC 'n'
# define WAITING 'w'
# define WAITINGW 'W'
# define RESTRICTEDROUTING 'T'
# define PAD 'p'
# define SIZEDATA 's'
# define TTL "--ttl"

enum helpoptEnum {NONE, HELP, USAGE};

typedef struct	parsedData_s {
	enum helpoptEnum	help;
	t_list*				domains;
	bool				verbose;
	bool				flood;
	bool				numeric;
	bool				restrictedRouting;
	long				load;
	unsigned int		waiting;
	long				waitingW;
	long				ttl;
	short				pad;
	size_t				sizeData;
}				parsedData_t;


void nullfunc(void* data);
void execHelpOption(parsedData_t* setup);
void parseInputPing(char **argv, parsedData_t* setup);
void parseInputPing2(int argc, char **argv, parsedData_t* setup);

#endif
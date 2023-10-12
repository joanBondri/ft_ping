#ifndef PARSER_H
# define PARSER_H
# include <stdbool.h>
# include "libft.h"

enum helpoptEnum {NONE, HELP, USAGE};

typedef struct	parsedData_s {
	enum helpoptEnum	help;
	bool				verbose;
	t_list*				domains;
	bool				flood;
	int					load;
	bool				numeric;
	uint				waiting;
	uint				waitingW;
	bool				restrictedRouting;
	char				pad[16];
	size_t				sizeData;
	uint				ttl;
}				parsedData_t;


void nullfunc(void* data);
void execHelpOption(parsedData_t* setup);
void parseInputPing(char **argv, parsedData_t* setup);

#endif
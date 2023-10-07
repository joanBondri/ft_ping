#ifndef PARSER_H
# define PARSER_H
# include <stdbool.h>
# include "libft.h"

enum helpoptEnum {NONE, HELP, USAGE};

typedef struct	parsedData_s {
	enum helpoptEnum	help;
	bool				verbose;
	t_list*				domains;
}				parsedData_t;


void nullfunc(void* data);
void execHelpOption(parsedData_t* setup);
void parseInputPing(char **argv, parsedData_t* setup);

#endif
#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	(void)argc;
	parsedData_t	setup;
	ft_memset((void*)&setup, 0, sizeof(setup));
	parseInputPing(argv, &setup);
	if (setup.help != NONE)
		execHelpOption(&setup);
	return (0);
}
#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void usageOptionMessage(void)
{
	const char str[] = "\
Usage: ft_ping [-v?] [--verbose] [--help] [--usage]\n\
            HOST ...";
	printf("%s\n", str);
	return ;
}

void helpOptionMessage(void)
{
	const char str[] = "\
Usage: ft_ping [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\
\n\
Options valid for all request types:\n\
  -v, --verbose              verbose output\n\
Options valid for --echo requests:\n\
  -?, --help                 give this help list\n\
      --usage                give a short usage message\n\
Mandatory or optional arguments to long options are also mandatory or optional\n\
for any corresponding short options.\n\
\n\
Report bugs to <jbondri@gmail.com>.";
	printf("%s\n", str);
	return ;
}

void execHelpOption(parsedData_t* setup)
{
	if (setup->help == HELP)
		helpOptionMessage();
	else if (setup->help == USAGE)
		usageOptionMessage();
	ft_lstclear(&(setup->domains), nullfunc);
	exit(0);
}

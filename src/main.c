#include "parser.h"
#include "creationRequest.h"
#include "libft.h"

void printParsedData(parsedData_t data) {
    printf("help: %d\n", data.help);
    printf("verbose: %d\n", data.verbose);
    printf("flood: %d\n", data.flood);
    printf("load: %d\n", data.load);
    printf("numeric: %d\n", data.numeric);
    printf("waiting: %u\n", data.waiting);
    printf("waitingW: %u\n", data.waitingW);
    printf("restrictedRouting: %d\n", data.restrictedRouting);
    printf("pad: %d\n", data.pad);
    printf("sizeData: %zu\n", data.sizeData);
    printf("ttl: %d\n", data.ttl);
}


int main (int argc, char **argv)
{
	(void)argc;
	parsedData_t	setup;
	ft_memset((void*)&setup, 0, sizeof(setup));
	parseInputPing2(argc, argv, &setup);
    printParsedData(setup);
	// if (setup.help != NONE)
	// 	execHelpOption(&setup);

    // for (t_list *one = setup.domains; one != NULL; one = one->next)
    // {
    //     if (creationOfRequest((const char*)one->content, setup))
    //     {
    //         ft_lstclear(&(setup.domains), nullfunc);
    //         exit(1);
    //     }
    // }
	return (0);
}
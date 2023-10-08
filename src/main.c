#include "parser.h"
#include "creationRequest.h"
#include "libft.h"

int main (int argc, char **argv)
{
	(void)argc;
	parsedData_t	setup;
	ft_memset((void*)&setup, 0, sizeof(setup));
	parseInputPing(argv, &setup);
	if (setup.help != NONE)
		execHelpOption(&setup);

    for (t_list *one = setup.domains; one != NULL; one = one->next)
    {
        if (creationOfRequest((const char*)one->content))
        {
            ft_lstclear(&(setup.domains), nullfunc);
            exit(1);
        }
    }

	return (0);
}
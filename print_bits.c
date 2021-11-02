#include "libft.h"
#include <stdlib.h>

int	main(int ac, char **av)
{
	uint16_t	nb = atoi(av[1]);
	ft_printf("nb = ");
	print_bits(&nb, 16);
	ft_printf(" (%d)", nb);
	nb = ~nb;
	ft_printf("\ninversing nb\n");
	ft_printf("nb = ");
	print_bits(&nb, 16);
	ft_printf(" (%d)", nb);
	ft_printf("\n");
	return (0);
}

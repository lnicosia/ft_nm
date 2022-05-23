#include "libft.h"

double	ft_centiceil(double nb)
{
    nb *= 10;
    nb = ft_ceil(nb);
    return nb / 10.0;
}

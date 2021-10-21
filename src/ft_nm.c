/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:13:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 13:23:54 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "nm.h"
#include "options.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int		map_file(char *file, int fd, t_stat stats, int opt)
{
	char				*ptr;

	if ((ptr = (char *)mmap(0, (size_t)stats.st_size,
		PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (custom_error("ft_nm: mmap error"));
	if (stats.st_size < 4)
		return (custom_error("ft_nm: Invalid file size (%ld)\n", stats.st_size));
	if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
		&& ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3)
	{
		//ft_printf("ELF binary\n");
		if (ptr[4] == ELFCLASS64)
		{
			//ft_printf("64-bits binary\n");
			handle_64(file, ptr, opt);
		}
		else if (ptr[4] == ELFCLASS32)
		{
			//ft_printf("32-bits binary\n");
		}
		else if (ptr[4] == ELFCLASSNONE)
			return (custom_error("ft_nm: Invalid file class\n"));
	}
	else
		return (custom_error("ft_nm: %s: File format not recognized\n", file));
	if (munmap(ptr, (size_t)stats.st_size))
		return (custom_error("ft_nm: munmap error"));
	return (0);
}

int		analyze_file(char *file, int opt)
{
	int		fd;
	t_stat	stats;

	fd = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
	{
		ft_printf("ft_nm: '%s': Open error", file);
		return (custom_error("\n"));
	}
	if (fstat(fd, &stats))
		return (custom_error("\n"));
	if (S_ISDIR(stats.st_mode))
	{
		ft_printf("ft_nm: Warning: '%s' is a directory\n", file);
		return (-1);
	}
	else if (!S_ISREG(stats.st_mode))
	{
		ft_printf("ft_nm: Warning: '%s' is not an ordinary file\n", file);
		return (-1);
	}
	map_file(file, fd, stats, opt);
	if (close(fd))
		return (custom_error("\n"));
	return (0);
}

int		ft_nm(int ac, char **av)
{
	int	i;
	int	opt;

	opt = 0;
	parse_nm_options(ac, av, &opt);
	if (ac > 2)
		opt |= OPT_PRINT_FILE_NAME;
	i = 1;
	while (i < ac)
	{
		if (!is_arg_an_option_line(av[i]))
			analyze_file(av[i], opt);
		i++;
	}
	if (ac == 1)
		analyze_file("a.out", opt);
	return (0);
}

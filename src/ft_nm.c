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

int		parse_file(char *ptr, char *file, t_stat stats, int opt)
{
	if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
		&& ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3)
	{
		if (ptr[5] == ELFDATA2MSB)
			opt |= OPT_BIG_ENDIAN;
		else
			opt |= OPT_LITTLE_ENDIAN;
		if (ptr[4] == ELFCLASS64)
		{
			if (opt & OPT_VERBOSE)
				ft_bprintf(0, "64-bits binary\n");
			handle_64(file, ptr, stats.st_size, opt);
		}
		else if (ptr[4] == ELFCLASS32)
		{
			if (opt & OPT_VERBOSE)
				ft_bprintf(0, "32-bits binary\n");
			handle_32(file, ptr, stats.st_size, opt);
		}
		else if (ptr[4] == ELFCLASSNONE)
			return (custom_error("ft_nm: Invalid file class\n"));
		else
			return (custom_error("ft_nm: %s: File format not recognized\n", file));
	}
	else
		return (custom_error("ft_nm: %s: File format not recognized\n", file));
	return (0);
}

int		map_file(char *file, int fd, t_stat stats, int opt)
{
	char	*ptr;
	int		ret;

	if ((ptr = (char *)mmap(0, (size_t)stats.st_size,
		PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (custom_error("ft_nm: mmap error"));
	ret = parse_file(ptr, file, stats, opt);
	if (munmap(ptr, (size_t)stats.st_size))
		return (custom_error("ft_nm: munmap error"));
	return (ret);
}

int		analyze_file(int fd, char *file, int opt)
{
	t_stat	stats;

	if (fstat(fd, &stats))
		return (custom_error("ft_nm: fstat error\n"));
	if (S_ISDIR(stats.st_mode))
		return (custom_error("ft_nm: Warning: '%s' is a directory\n", file));
	else if (!S_ISREG(stats.st_mode))
		return (custom_error("ft_nm: Warning: '%s' is not an ordinary file\n", file));
	if (opt & OPT_VERBOSE)
		ft_bprintf(0, "File size = %d\n", stats.st_size);
	if (stats.st_size == 0)
		return (-1);
	if ((unsigned int)stats.st_size <= 3)
		return (custom_error("ft_nm: %s: File truncated\n", file));
	if (map_file(file, fd, stats, opt))
		return (-1);
	return (0);
}

int		open_file(char *file, int opt)
{
	int		fd;

	fd = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		return (custom_error("ft_nm: '%s': Open error\n", file));
	if (analyze_file(fd, file, opt))
		return (-1);
	if (close(fd))
		return (custom_error("ft_nm: Close error\n"));
	return (0);
}

int		ft_nm(int ac, char **av)
{
	int	i;
	int	nb_files;
	int	opt;
	int	ret;

	opt = 0;
	nb_files = 0;
	ret = parse_nm_options(ac, av, &opt, &nb_files);
	if (ret == 1)
		return (1);
	else if (ret == 2)
		return (0);
	if (!(opt & OPT_POSIX) && !(opt & OPT_SYSV))
		opt |= OPT_BSD;
	if (nb_files > 1 && !(opt & OPT_O))
		opt |= OPT_PRINT_FILE_NAME;
	i = 1;
	while (i < ac)
	{
		if (!is_arg_an_option_line(av[i]))
		{
			if (open_file(av[i], opt) != 0)
				ret = 1;
		}
		i++;
	}
	if (nb_files == 0)
	{
		if (open_file("a.out", opt))
			return (1);
	}
	ft_bprintf(1, "");
	return (ret);
}

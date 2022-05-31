/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_option_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 15:30:41 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/23 09:18:10 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "libft.h"

int		print_version(void)
{
	ft_printf("lnicosia's ft_nm version 1.0\n");
	ft_printf("This program is free software; you may redistribute it\n");
	ft_printf("This program has absolutely no warranty\n");
	return (2);
}

int		print_usage_stdin(void)
{
	ft_printf("Usage: ft_nm [option(s)] [file(s)]\n");
	ft_printf(" List symbols in [file(s)] (a.out by default).\n");
	ft_printf(" The options are:\n");
	ft_printf("  -a, --debug-syms\tDisplay debugger-only symbols\n");
	ft_printf("  -A, --print-file-name\tPrint name of the input file before every symbol\n");
	ft_printf("  -B\t\t\tSame as --format=bsd\n");
	//ft_printf("  -D, --dynamic\t\tDisplay dynamic symbols instead of normal symbols\n");
	ft_printf("      --defined-only\tDisplay only defined symbols\n");
	ft_printf("  -e\t\t\t(ignored)\n");
	ft_printf("      --format=FORMAT\tUse the output format FORMAT. Format cat be `bsd`,\n");
	ft_printf("\t\t\t  `sysv' or `posix'. The default is `bsd'\n");
	ft_printf("  -n, --numeric-sort\tSort symbols numerically by address\n");
	ft_printf("  -o\t\t\tSame as -A\n");
	ft_printf("  -p, --no-sort\t\tDo no sort the symbols\n");
	ft_printf("  -P, --portability\tSame as --format=posix\n");
	ft_printf("  -r, --reverse-sort\tReverse the sense of the sort\n");
	ft_printf("  -S, --print-size\tPrint size of defined symbols\n");
	ft_printf("      --size-sort\tSort symbols by size\n");
	ft_printf("  -u, --undefined-only\tDisplay only undefined symbols\n");
	ft_printf("  -h, --help\t\tDisplay this information\n");
	ft_printf("      --verbose\t\tDisplay additional information about the symbols\n");
	ft_printf("  -V, --version\t\tDisplay this program's version number\n");
	ft_printf("  -v\t\t\tSame as -n\n");
	ft_printf("\nft_nm: supported targets: x86-64 x64, object files, .so\n");
	ft_printf("Report bugs to <https://github.com/lnicosia/ft_nm>.\n");
	return (2);
}

int		print_usage(void)
{
	custom_error("Usage: ft_nm [option(s)] [file(s)]\n");
	custom_error(" List symbols in [file(s)] (a.out by default).\n");
	custom_error(" The options are:\n");
	custom_error("  -a, --debug-syms\tDisplay debugger-only symbols\n");
	custom_error("  -A, --print-file-name\tPrint name of the input file before every symbol\n");
	custom_error("  -B\t\t\tSame as --format=bsd\n");
	//custom_error("  -D, --dynamic\t\tDisplay dynamic symbols instead of normal symbols\n");
	custom_error("      --defined-only\tDisplay only defined symbols\n");
	custom_error("  -e\t\t\t(ignored)\n");
	custom_error("      --format=FORMAT\tUse the output format FORMAT. Format cat be `bsd`,\n");
	custom_error("\t\t\t  `sysv' or `posix'. The default is `bsd'\n");
	custom_error("  -n, --numeric-sort\tSort symbols numerically by address\n");
	custom_error("  -o\t\t\tSame as -A\n");
	custom_error("  -p, --no-sort\t\tDo no sort the symbols\n");
	custom_error("  -P, --portability\tSame as --format=posix\n");
	custom_error("  -r, --reverse-sort\tReverse the sense of the sort\n");
	custom_error("  -S, --print-size\tPrint size of defined symbols\n");
	custom_error("      --size-sort\tSort symbols by size\n");
	custom_error("  -u, --undefined-only\tDisplay only undefined symbols\n");
	custom_error("  -h, --help\t\tDisplay this information\n");
	custom_error("      --verbose\t\tDisplay additional information about the symbols\n");
	custom_error("  -v\t\t\tSame as -n\n");
	custom_error("  -V, --version\t\tDisplay this program's version number\n");
	custom_error("\nft_nm: supported targets: x86-64 x64, object files, .so\n");
	custom_error("Report bugs to <https://github.com/lnicosia/ft_nm>.\n");
	return (1);
}

int		check_opt(char *av, int *opt)
{
	if (*av == 'a')
		*opt |= OPT_A;
	else if (*av == 'p')
	{
		*opt |= OPT_P;
		*opt &= ~OPT_N;
		*opt &= ~OPT_SIZE_SORT;
	}
	else if (*av == 'S')
		*opt |= OPT_S;
	else if (*av == 'u')
		*opt |= OPT_U;
//	else if (*av == 'D')
//		*opt |= OPT_D;
	else if (*av == 'r')
		*opt |= OPT_R;
	else if (*av == 'n')
	{
		*opt |= OPT_N;
		*opt &= ~OPT_P;
		*opt &= ~OPT_SIZE_SORT;
	}
	else if (*av == 'v')
	{
		*opt |= OPT_N;
		*opt &= ~OPT_P;
		*opt &= ~OPT_SIZE_SORT;
	}
	else if (*av == 'B')
		*opt |= OPT_BSD;
	else if (*av == 'P')
		*opt |= OPT_POSIX;
	else if (*av == 'o' || *av == 'A')
		*opt |= OPT_O;
	else if (*av == 'h')
		return (print_usage_stdin());
	else if (*av == 'V')
		return (print_version());
	else if (*av != 'e')
	{
		custom_error("ft_nm: invalid option -- '%s'\n", av);
		return (print_usage());
	}
	return (0);
}

int		parse_option_line(char *av, int *opt)
{
	int	ret;

	if (ft_strbegin(av, "--"))
	{
		if (ft_strequ(av, "--verbose"))
			*opt |= OPT_VERBOSE;
		else if (ft_strnequ(av, "--debug-syms", ft_strlen(av)))
			*opt |= OPT_A;
		else if (ft_strnequ(av, "--print-file-name", ft_strlen(av)))
			*opt |= OPT_O;
		//else if (ft_strnequ(av, "--dynamic", ft_strlen(av)))
		//	*opt |= OPT_D;
		else if (ft_strnequ(av, "--defined-only", ft_strlen(av)))
			*opt |= OPT_DE;
		else if (ft_strbegin(av, "--format="))
		{
			if (ft_strbegin(av, "--format=b"))
				*opt |= OPT_BSD;
			else if (ft_strbegin(av, "--format=s"))
				*opt |= OPT_SYSV;
			else if (ft_strbegin(av, "--format=p"))
				*opt |= OPT_POSIX;
			else
				return (custom_error("ft_nm: invalid output format\n"));
		}
		else if (ft_strnequ(av, "--numeric-sort", ft_strlen(av)))
			*opt |= OPT_N;
		else if (ft_strnequ(av, "--no-sort", ft_strlen(av)))
			*opt |= OPT_P;
		else if (ft_strnequ(av, "--portability", ft_strlen(av)))
			*opt |= OPT_POSIX;
		else if (ft_strnequ(av, "--reverse-sort", ft_strlen(av)))
			*opt |= OPT_R;
		else if (ft_strnequ(av, "--print-size", ft_strlen(av)))
			*opt |= OPT_S;
		else if (ft_strnequ(av, "--size-sort", ft_strlen(av)))
		{
			*opt |= OPT_SIZE_SORT;
			*opt &= ~OPT_N;
			*opt &= ~OPT_P;
		}
		else if (ft_strnequ(av, "--undefined-only", ft_strlen(av)))
			*opt |= OPT_U;
		else if (ft_strnequ(av, "--help", ft_strlen(av)))
			return (print_usage_stdin());
		else if (ft_strnequ(av, "--version", ft_strlen(av)))
			return (print_version());
		else
		{
			custom_error("ft_nm: unrecognized option '%s'\n", av);
			return (print_usage());
		}
	}
	else
	{
		av++;
		while (*av)
		{
			if ((ret = check_opt(av, opt)) != 0)
				return (ret);
			av++;
		}
	}
	return (0);
}

/*
**	Checks if the given string is an option line (starting with '-')
*/

int		is_arg_an_option_line(char *av)
{
	return (ft_strlen(av) > 1 && av[0] == '-');
}

/*
**	Parse all the options by checking arguments starting with '-'
*/

int		parse_nm_options(int ac, char **av, int *opt, int* nb_files)
{
	int	i;
	int	ret;
	
	i = 1;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			if ((ret = parse_option_line(av[i], opt)) != 0)
				return (ret);
		}
		else
			(*nb_files)++;
		i++;
	}
	return (0);
}

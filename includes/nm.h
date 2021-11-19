/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:11:46 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 18:05:56 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
# include <elf.h>
# include "libft.h"

enum				e_sym_types
{
	r,
	R,
	B,
	d,
	D,
	T,
	t,
	w,
	W
};

typedef struct stat	t_stat;

typedef struct		s_sym64
{
	Elf64_Sym		sym;
	char			*name;
	int				type;
	char			padding[4];
}					t_sym64;

typedef struct		s_sym32
{
	Elf32_Sym		sym;
	char			*name;
	int				type;
	char			padding[4];
}					t_sym32;

int		ft_nm(int ac, char **av);
int		parse_nm_options(int ac, char **av, int *opt);
int		is_arg_an_option_line(char *av);
void	handle_64(char *file, char *ptr, long int file_size, int opt);
void	handle_32(char *file, char *ptr, long int file_size, int opt);
int		parse_option_line(char *av, int *opt);

//		Comparison functions used to sort symbols
int 	(*set_compare_func(int type, int opt))(void *, void *);
int		no_sort(void *s1, void *s2);
int		compare_sizes32(void *s1, void *s2);
int		compare_sizes64(void *s1, void *s2);
int		compare_names32(void *s1, void *s2);
int		compare_names64(void *s1, void *s2);
int		compare_addresses32(void *s1, void *s2);
int		compare_addresses64(void *s1, void *s2);
int		compare_names_no_special(void *s1, void *s2);

//		Reverse comparison function (-r)
//		We need need versions because it's not
//		exactly a reverse (addresses are still sorted
//		ascending)
int		rno_sort(void *s1, void *s2);
int		rcompare_sizes32(void *s1, void *s2);
int		rcompare_sizes64(void *s1, void *s2);
int		rcompare_names32(void *s1, void *s2);
int		rcompare_names64(void *s1, void *s2);
int		rcompare_addresses32(void *s1, void *s2);
int		rcompare_addresses64(void *s1, void *s2);
int		rcompare_names_no_special(void *s1, void *s2);

void	print_64_bsd_symbols(t_dlist *lst, char *file, char *ptr,
Elf64_Ehdr *header, Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt);
void	print_32_bsd_symbols(t_dlist *lst, char *file, char *ptr,
Elf32_Ehdr *header, Elf32_Shdr *shstr, Elf32_Shdr *shstrhdr, int opt);
void	print_64_sysv_symbols(t_dlist *lst, char *file, char *ptr,
Elf64_Ehdr *header, Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt);
void	print_32_sysv_symbols(t_dlist *lst, char *file, char *ptr,
Elf32_Ehdr *header, Elf32_Shdr *shstr, Elf32_Shdr *shstrhdr, int opt);
void	print_64_posix_symbols(t_dlist *lst, char *file, char *ptr,
Elf64_Ehdr *header, Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt);
void	print_32_posix_symbols(t_dlist *lst, char *file, char *ptr,
Elf32_Ehdr *header, Elf32_Shdr *shstr, Elf32_Shdr *shstrhdr, int opt);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:11:46 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 11:53:51 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
# include <elf.h>

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

typedef struct		s_sym
{
	Elf64_Sym		*sym;
	char			*name;
	int				type;
	char			padding[4];
}					t_sym;


int		ft_nm(int ac, char **av);
int		parse_nm_options(int ac, char **av, int *opt);
int		is_arg_an_option_line(char *av);
void	handle_64(char *ptr, int opt);
int		parse_option_line(char *av, int *opt);
int		compare_names(void *s1, void *s2);

#endif
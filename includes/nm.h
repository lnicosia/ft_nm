/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:11:46 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/06 15:41:56 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
# include <elf.h>

typedef struct stat	t_stat;

typedef struct		s_sym
{
	Elf64_Sym		*sym;
	char			*name;
}					t_sym;


int		ft_nm(int ac, char **av);
void	handle_64(char *ptr, int opt);
int		parse_option_line(char *av, int *opt);
int		compare_names(void *s1, void *s2);

#endif
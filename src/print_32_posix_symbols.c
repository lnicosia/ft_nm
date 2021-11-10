/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_32_posix_symbols.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 16:05:36 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/23 09:21:42 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "options.h"
#include "read_data.h"
#include <limits.h>

void	print_32_posix_symbols(t_dlist *lst, char *file, char *ptr, Elf32_Ehdr *header,
Elf32_Shdr *shstr, Elf32_Shdr *shstrhdr, int opt)
{
	t_sym32		*sym;
	int			padding;

	(void)shstr;
	(void)shstrhdr;
	(void)header;
	padding = 8;
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		sym = (t_sym32*)lst->content;
		if ((opt & OPT_D
			&& ELF32_ST_BIND(sym->sym.st_info) != STB_GLOBAL
			&& ELF32_ST_BIND(sym->sym.st_info) != STB_WEAK))
		{
			lst = lst->next;
			continue ;
		}
		if (opt & OPT_O)
			ft_printf("%s: ", file);
		if (opt & OPT_D)
		{
			ft_printf(" ");
			int i = 0;
			while (sym->name[i] && sym->name[i] != '@')
			{
				ft_printf("%c", sym->name[i]);
				i++;
			}
		}
		else
			ft_printf("%s", sym->name);
		ft_printf(" %c", sym->type);
		if (sym->type == 'C')
			ft_printf(" %0*x", padding, read_uint32(sym->sym.st_size, opt));
		else if (ft_strstr(ptr + read_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
			|| ft_strstr(ptr + read_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_printf(" %0*x", padding / 2, read_unsigned_int(sym->sym.st_value, opt));
		else
		{
			if (read_uint16(sym->sym.st_shndx, opt) != 0)
				ft_printf(" %0*x", padding, read_unsigned_int(sym->sym.st_value, opt));
			else
				ft_printf(" %8s", "");
		}
		if (read_uint32(sym->sym.st_size, opt) && read_uint16(sym->sym.st_shndx, opt) != 0)
			ft_printf(" %0*x", padding, read_uint32(sym->sym.st_size, opt));
		else if (read_uint16(sym->sym.st_shndx, opt) != 0)
			ft_printf(" ");
		ft_printf("\n");
		lst = lst->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_64_posix_symbols.c                           :+:      :+:    :+:   */
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

void	print_64_posix_symbols(t_dlist *lst, char *file, char *ptr, Elf64_Ehdr *header,
Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt)
{
	t_sym64		*sym;
	int			padding;

	(void)shstr;
	(void)shstrhdr;
	(void)header;
	if (opt & OPT_LTO)
		padding = 8;
	else
		padding = 16;
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		sym = (t_sym64*)lst->content;
		if ((opt & OPT_D && sym->sym.st_shndx != 0)
			|| (opt & OPT_U && sym->type != 'U' && sym->type != 'w'
				&& sym->type != 'v')
			|| (opt & OPT_G && ((sym->type != 'u' && sym->type != 'U' && sym->type != 'w'
				&& sym->type != 'v' && sym->type != 'T' && sym->type != 'W'
				&& sym->type != 'B' && sym->type != 'D' && sym->type != 'R'
				&& sym->type != 'A' && sym->type != 'V' && sym->type != 'i'
				&& sym->type != 'C')
				|| (sym->type == 'i' && ELF64_ST_BIND(sym->sym.st_info) == STB_LOCAL)))
			|| (opt & OPT_SIZE_SORT && (sym->sym.st_size == 0 || sym->type == 'U')))
		//	&& ELF64_ST_BIND(sym->sym.st_info) != STB_GLOBAL
		//	&& ELF64_ST_BIND(sym->sym.st_info) != STB_WEAK))
		{
			lst = lst->next;
			continue ;
		}
		if (opt & OPT_O)
			ft_bprintf(0, "%s: ", file);
		if (opt & OPT_D)
		{
			ft_bprintf(0, " ");
			int i = 0;
			while (sym->name[i] && sym->name[i] != '@')
			{
				ft_bprintf(0, "%c", sym->name[i]);
				i++;
			}
		}
		else
			ft_bprintf(0, "%s", sym->name);
		ft_bprintf(0, " %c", sym->type);
		if (ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
			|| ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_bprintf(0, " %*x%0*x", padding / 2, 0xffffffff, padding / 2, sym->sym.st_value);
		else
		{
			if (sym->sym.st_shndx != 0)
				ft_bprintf(0, " %0*x", padding, sym->sym.st_value);
			else
				ft_bprintf(0, " %8s", "");
		}
		if (sym->sym.st_size && sym->sym.st_shndx != 0)
			ft_bprintf(0, " %0*x", padding, sym->sym.st_size);
		else if (sym->sym.st_shndx != 0)
			ft_bprintf(0, " ");
		ft_bprintf(0, "\n");
		lst = lst->next;
	}
}

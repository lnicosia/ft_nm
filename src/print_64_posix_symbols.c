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
		if ((opt & OPT_D && sym->sym.st_shndx != 0))
		//	&& ELF64_ST_BIND(sym->sym.st_info) != STB_GLOBAL
		//	&& ELF64_ST_BIND(sym->sym.st_info) != STB_WEAK))
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
		if (ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
			|| ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_printf(" %*x%0*x", padding / 2, 0xffffffff, padding / 2, sym->sym.st_value);
		else
		{
			if (sym->sym.st_shndx != 0)
				ft_printf(" %0*x", padding, sym->sym.st_value);
			else
				ft_printf(" %8s", "");
		}
		if (read_uint64(sym->sym.st_size, opt) && sym->sym.st_shndx != 0)
			ft_printf(" %0*x", padding, read_uint64(sym->sym.st_size, opt));
		else if (sym->sym.st_shndx != 0)
			ft_printf(" ");
		ft_printf("\n");
		lst = lst->next;
	}
}

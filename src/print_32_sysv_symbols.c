/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_32_sysv_symbols.c                            :+:      :+:    :+:   */
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

void	print_32_sysv_symbols(t_dlist *lst, char *file, char *ptr, Elf32_Ehdr *header,
		Elf32_Shdr *shstr, Elf32_Shdr *shstrhdr, int opt)
{
	t_sym32		*sym;
	int			shndx_ok;
	int			padding;

	(void)shstr;
	padding = 8;
	while (lst && lst->prev)
		lst = lst->prev;
	ft_bprintf(0, "\n\n");
	if (opt & OPT_U)
		ft_bprintf(0, "Undefined s");
	else
		ft_bprintf(0, "S");
	ft_bprintf(0, "ymbols from %s:\n\n", file);
	ft_bprintf(0, "%-22s%-*s%-13s%-13s%-*s%-6s%s\n\n", "Name", padding, "Value", "Class", "Type",
			padding + 1, "Size", "Line", "Section");
	while (lst)
	{
		sym = (t_sym32*)lst->content;
		if ((opt & OPT_D
				&& ELF32_ST_BIND(sym->sym.st_info) != STB_GLOBAL
				&& ELF32_ST_BIND(sym->sym.st_info) != STB_WEAK)
			|| (opt & OPT_U && sym->type != 'U' && sym->type != 'w'
				&& sym->type != 'v')
			|| (opt & OPT_G && ((sym->type != 'u' && sym->type != 'U' && sym->type != 'w'
				&& sym->type != 'v' && sym->type != 'T' && sym->type != 'W'
				&& sym->type != 'B' && sym->type != 'D' && sym->type != 'R'
				&& sym->type != 'A' && sym->type != 'V' && sym->type != 'i'
				&& sym->type != 'C')
				|| (sym->type == 'i' && ELF64_ST_BIND(sym->sym.st_info) == STB_LOCAL)))
			|| (opt & OPT_SIZE_SORT && (sym->sym.st_size == 0 || sym->type == 'U')))
		{
			lst = lst->next;
			continue ;
		}
		if (sym->sym.st_shndx == 0
			|| sym->sym.st_shndx >= read_uint16(header->e_shnum, opt))
			shndx_ok = 0;
		else
			shndx_ok = 1;
		Elf32_Shdr	*sheader = (Elf32_Shdr*) (ptr + read_unsigned_int(header->e_shoff, opt)
				+ (read_uint16(header->e_shentsize, opt) * sym->sym.st_shndx));
		if (opt & OPT_O)
			ft_bprintf(0, "%s:", file);
		if (opt & OPT_D)
		{
			int i = 0;
			while (sym->name[i] && sym->name[i] != '@')
			{
				ft_bprintf(0, "%c", sym->name[i]);
				i++;
			}
			ft_bprintf(0, "%-*d", 20 - i); 
		}
		else
			ft_bprintf(0, "%-20s", sym->name);
		if (ft_strstr(ptr + read_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
				|| ft_strstr(ptr + read_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_bprintf(0, "|%0*x|", padding / 2, sym->sym.st_value);
		else
		{
			if (sym->sym.st_shndx != 0)
				ft_bprintf(0, "|%0*x|", padding, sym->sym.st_value);
			else
				ft_bprintf(0, "|%*s|", padding, "");
		}
		ft_bprintf(0, "   %c  ", sym->type);
		char	*type = NULL;
		switch (ELF32_ST_TYPE(sym->sym.st_info))
		{
			case STT_NOTYPE:
				if (opt & OPT_LTO)
					type = "";
				else	
					type = " NOTYPE";
				break ;
			case STT_OBJECT:
				type = " OBJECT";
				break ;
			case STT_FUNC:
				type = " FUNC";
				break ;
			case STT_SECTION:
				type = " SECTION";
				break ;
			case STT_FILE:
				type = " FILE";
				break ;
			case STT_LOPROC:
				type = " LOPROC";
				break ;
			case STT_HIPROC:
				type = " HIPROC";
				break ;
			case STT_COMMON:
				type = " COMMON";
				break ;
			case STT_LOOS:
				type = " <OS specific>: 10";
				break ;
			case STT_HIOS:
				type = " HOS";
				break ;
			case STT_TLS:
				type = " TLS";
				break ;
		}
		if (ELF32_ST_TYPE(sym->sym.st_info) != STT_SECTION)
			ft_bprintf(0, "|%18s|", type);
		else
			ft_bprintf(0, "|%18s|", "");
		if (sym->sym.st_size)
			ft_bprintf(0, "%0*x|", padding, sym->sym.st_size);
		else
			ft_bprintf(0, "%*s|", padding, "");
		ft_bprintf(0, "     |");
		if (shndx_ok && !(opt & OPT_LTO))
		{
			if (ELF32_ST_TYPE(sym->sym.st_info) != STT_SECTION)
				ft_bprintf(0, "%s", ptr
				+ read_unsigned_int(shstrhdr->sh_offset, opt)
				+ read_uint32(sheader->sh_name, opt));
		}
		else if (!(opt & OPT_LTO))
		{
			switch (sym->sym.st_shndx)
			{
				case SHN_UNDEF:
					ft_bprintf(0, "*UND*");
					break ;
				case SHN_ABS:
					ft_bprintf(0, "*ABS*");
					break ;
				case SHN_COMMON:
					ft_bprintf(0, "*COM*");
					break ;
				default:
					ft_bprintf(0, "???");
					break;
			}
		}
		ft_bprintf(0, "\n");
		lst = lst->next;
	}
}

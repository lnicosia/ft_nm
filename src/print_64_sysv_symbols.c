/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_64_sysv_symbols.c                            :+:      :+:    :+:   */
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

void	print_64_sysv_symbols(t_dlist *lst, char *file, char *ptr, Elf64_Ehdr *header,
		Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt)
{
	t_sym64		*sym;
	int			shndx_ok;
	int			padding;

	(void)shstr;
	if (opt & OPT_LTO)
		padding = 8;
	else
		padding = 16;
	while (lst && lst->prev)
		lst = lst->prev;
	ft_printf("\n\n");
	ft_printf("Symbols from %s:\n\n", file);
	ft_printf("%-22s%-*s%-13s%-13s%-*s%-6s%s\n\n", "Name", padding, "Value", "Class", "Type",
			padding + 1, "Size", "Line", "Section");
	while (lst)
	{
		sym = (t_sym64*)lst->content;
		if ((opt & OPT_D && read_uint16(sym->sym.st_shndx, opt) != 0))
			//	&& ELF64_ST_BIND(sym->sym.st_info) != STB_GLOBAL
			//	&& ELF64_ST_BIND(sym->sym.st_info) != STB_WEAK))
		{
			lst = lst->next;
			continue ;
		}
		if (read_uint16(sym->sym.st_shndx, opt) == 0
			|| read_uint16(sym->sym.st_shndx, opt) >= read_uint16(header->e_shnum, opt))
			shndx_ok = 0;
		else
			shndx_ok = 1;
		Elf64_Shdr	*sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
				+ (read_uint16(header->e_shentsize, opt) * read_uint16(sym->sym.st_shndx, opt)));
		if (opt & OPT_O)
			ft_printf("%s:", file);
		if (opt & OPT_D)
		{
			int i = 0;
			while (sym->name[i] && sym->name[i] != '@')
			{
				ft_printf("%c", sym->name[i]);
				i++;
			}
			ft_printf("%-*d", 20 - i); 
		}
		else
			ft_printf("%-20s", sym->name);
		if (ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
				|| ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_printf("|%*x%0*x|", padding / 2, 0xffffffff, padding / 2, read_long_unsigned_int(sym->sym.st_value, opt));
		else
		{
			if (read_uint16(sym->sym.st_shndx, opt) != 0)
				ft_printf("|%0*x|", padding, read_long_unsigned_int(sym->sym.st_value, opt));
			else
				ft_printf("|%*s|", padding, "");
		}
		ft_printf("   %c  ", sym->type);
		char	*type = NULL;
		switch (ELF64_ST_TYPE(sym->sym.st_info))
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
		ft_printf("|%18s|", type);
		if (read_uint64(sym->sym.st_size, opt))
			ft_printf("%0*x|", padding, read_uint64(sym->sym.st_size, opt));
		else
			ft_printf("%*s|", padding, "");
		ft_printf("     |");
		if (shndx_ok && !(opt & OPT_LTO))
		{
			ft_printf("%s",
			ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt));
		}
		else if (!(opt & OPT_LTO))
		{
			switch (read_uint16(sym->sym.st_shndx, opt))
			{
				case SHN_UNDEF:
					ft_printf("*UND*");
					break ;
				case SHN_ABS:
					ft_printf("*ABS*");
					break ;
				case SHN_COMMON:
					ft_printf("*COM*");
					break ;
				default:
					ft_printf("???");
					break;
			}
		}
		ft_printf("\n");
		lst = lst->next;
	}
}

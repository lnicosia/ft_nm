/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_32_bsd_symbols.c                             :+:      :+:    :+:   */
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

void	print_32_bsd_symbols(t_dlist *lst, char *file, char *ptr, Elf32_Ehdr *header,
Elf32_Shdr *shstr, Elf32_Shdr *shstrhdr, int opt)
{
	t_sym32		*sym;
	int			shndx_ok;
	int			padding;

	(void)shstr;
	padding = 8;
	while (lst && lst->prev)
		lst = lst->prev;
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
		if (read_uint32(sym->sym.st_shndx, opt) >= read_uint16(header->e_shnum, opt))
			shndx_ok = 0;
		else
			shndx_ok = 1;
		Elf32_Shdr	*sheader = (Elf32_Shdr*) (ptr + read_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * sym->sym.st_shndx));
		if (opt & OPT_O)
			ft_bprintf(0, "%s:", file);
		if (opt & OPT_SIZE_SORT)
			ft_bprintf(0, "%*0x", padding, sym->sym.st_size);
		else
		{
			if (ft_strstr(ptr + read_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
				|| ft_strstr(ptr + read_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
				ft_bprintf(0, "%0*x", padding / 2, sym->sym.st_value);
			else
			{
				if (sym->sym.st_shndx != 0)
					ft_bprintf(0, "%0*x", padding, sym->sym.st_value);
				else
					ft_bprintf(0, "%*s", padding, "");
			}
			if (opt & OPT_S && sym->sym.st_size != 0 && sym->type != 'U')
				ft_bprintf(0, " %*0x", padding, sym->sym.st_size);
		}
		if (opt & OPT_VERBOSE)
			ft_bprintf(0, " %3d", sym->sym.st_info);
		ft_bprintf(0, " %c", sym->type);
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
			ft_bprintf(0, " %s", sym->name);
		if (opt & OPT_VERBOSE)
		{
			ft_bprintf(0, " T =");
			switch (ELF32_ST_TYPE(sym->sym.st_info))
			{
				case STT_NOTYPE:
					ft_bprintf(0, " NOTYPE");
					break ;
				case STT_OBJECT:
					ft_bprintf(0, " OBJECT");
					break ;
				case STT_FUNC:
					ft_bprintf(0, " FUNC");
					break ;
				case STT_SECTION:
					ft_bprintf(0, " SECTION");
					break ;
				case STT_FILE:
					ft_bprintf(0, " FILE");
					break ;
				case STT_LOPROC:
					ft_bprintf(0, " LOPROC");
					break ;
				case STT_HIPROC:
					ft_bprintf(0, " HIPROC");
					break ;
				case STT_COMMON:
					ft_bprintf(0, " COMMON");
					break ;
				case STT_LOOS:
					ft_bprintf(0, " LOOS");
					break ;
				case STT_HIOS:
					ft_bprintf(0, " HOS");
					break ;
			}
			ft_bprintf(0, " (%d)", ELF32_ST_TYPE(sym->sym.st_info));
			ft_bprintf(0, ", B =");
			switch (ELF32_ST_BIND(sym->sym.st_info))
			{
				case STB_LOCAL:
					ft_bprintf(0, " LOCAL");
					break ;
				case STB_GLOBAL:
					ft_bprintf(0, " GLOBAL");
					break ;
				case STB_WEAK:
					ft_bprintf(0, " WEAK");
					break ;
				case STB_LOPROC:
					ft_bprintf(0, " LOPROC");
					break ;
				case STB_HIPROC:
					ft_bprintf(0, " HIPROC");
					break ;
				case STB_LOOS:
					ft_bprintf(0, " LOOS");
					break ;
				case STB_HIOS:
					ft_bprintf(0, " HIOS");
					break ;
			}
			ft_bprintf(0, " (%d)", ELF32_ST_BIND(sym->sym.st_info));
			ft_bprintf(0, ", O = %d", ELF32_ST_VISIBILITY(sym->sym.st_other));
			ft_bprintf(0, ", S = %d", sym->sym.st_size);
			ft_bprintf(0, ", H = %d", sym->sym.st_shndx);
			if (shndx_ok)
			{
				ft_bprintf(0, " (%s)",
				ptr + read_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt));
				switch (read_uint32(sheader->sh_type, opt))
				{
					case SHT_NULL:
						ft_bprintf(0, " (NULL))");
						break ;
					case SHT_PROGBITS:
						ft_bprintf(0, " (PROGBITS))");
						break ;
					case SHT_SYMTAB:
						ft_bprintf(0, " (SYMTAB))");
						break ;
					case SHT_STRTAB:
						ft_bprintf(0, " (STRTAB))");
						break ;
					case SHT_RELA:
						ft_bprintf(0, " (RELA))");
						break ;
					case SHT_HASH:
						ft_bprintf(0, " (HASH))");
						break ;
					case SHT_DYNAMIC:
						ft_bprintf(0, " (DYNAMIC))");
						break ;
					case SHT_NOTE:
						ft_bprintf(0, " (NOTE))");
						break ;
					case SHT_NOBITS:
						ft_bprintf(0, " (NOBITS))");
						break ;
					case SHT_REL:
						ft_bprintf(0, " (REL))");
						break ;
					case SHT_SHLIB:
						ft_bprintf(0, " (SHLIB))");
						break ;
					case SHT_DYNSYM:
						ft_bprintf(0, " (DYNSYM))");
						break ;
					case SHT_LOPROC:
						ft_bprintf(0, " (LOPROC))");
						break ;
					case SHT_HIPROC:
						ft_bprintf(0, " (HIPROC))");
						break ;
					case SHT_LOUSER:
						ft_bprintf(0, " (LOUSER))");
						break ;
					case SHT_HIUSER:
						ft_bprintf(0, " (HIUSER))");
						break ;
				}
				ft_bprintf(0, "flags =");
				if (read_uint32(sheader->sh_flags, opt) & SHF_WRITE)
					ft_bprintf(0, " WRITE");
				if (read_uint32(sheader->sh_flags, opt) & SHF_ALLOC)
					ft_bprintf(0, " ALLOC");
				if (read_uint32(sheader->sh_flags, opt) & SHF_EXECINSTR)
					ft_bprintf(0, " EXECINSTR");
				if (read_uint32(sheader->sh_flags, opt) & SHF_MASKPROC)
					ft_bprintf(0, " MASKPROC");
				if (read_uint32(sheader->sh_flags, opt) & SHF_MERGE)
					ft_bprintf(0, " MERGE");
				if (read_uint32(sheader->sh_flags, opt) & SHF_STRINGS)
					ft_bprintf(0, " STRINGS");
				if (read_uint32(sheader->sh_flags, opt) & SHF_INFO_LINK)
					ft_bprintf(0, " INFO_LINK");
				if (read_uint32(sheader->sh_flags, opt) & SHF_LINK_ORDER)
					ft_bprintf(0, " LINK_ORDER");
				if (read_uint32(sheader->sh_flags, opt) & SHF_OS_NONCONFORMING)
					ft_bprintf(0, " OS_NONCONFORMING");
				if (read_uint32(sheader->sh_flags, opt) & SHF_GROUP)
					ft_bprintf(0, " GROUP");
				if (read_uint32(sheader->sh_flags, opt) & SHF_TLS)
					ft_bprintf(0, " TLS");
				if (read_uint32(sheader->sh_flags, opt) & SHF_MASKOS)
					ft_bprintf(0, " MASKOS");
				if (read_uint32(sheader->sh_flags, opt) & SHF_MIPS_MERGE)
					ft_bprintf(0, " MIPS_MERGE");
				if (read_uint32(sheader->sh_flags, opt) & SHF_ORDERED)
					ft_bprintf(0, " ORDERED");
				if (read_uint32(sheader->sh_flags, opt) & SHF_EXCLUDE)
					ft_bprintf(0, " EXCLUDE");
				ft_bprintf(0, " (%d)", read_uint32(sheader->sh_flags, opt));
			}
			else
			{
				switch (sym->sym.st_shndx)
				{
					case SHN_UNDEF:
					ft_bprintf(0, " (UNDEF)");
						break ;
					case SHN_ABS:
						ft_bprintf(0, " (ABS)");
						break ;
					case SHN_COMMON:
						ft_bprintf(0, " (COMMON)");
						break ;
				}
			}
		}
		ft_bprintf(0, "\n");
		lst = lst->next;
	}
}

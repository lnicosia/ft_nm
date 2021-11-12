/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_64_bsd_symbols.c                             :+:      :+:    :+:   */
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

void	print_64_bsd_symbols(t_dlist *lst, char *file, char *ptr, Elf64_Ehdr *header,
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
		if (sym->sym.st_shndx >= read_uint16(header->e_shnum, opt))
			shndx_ok = 0;
		else
			shndx_ok = 1;
		Elf64_Shdr	*sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * sym->sym.st_shndx));
		if (opt & OPT_O)
			ft_printf("%s:", file);
		if (ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
			|| ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_printf("%*x%0*x", padding / 2, 0xffffffff, padding / 2, sym->sym.st_value);
		else
		{
			if (sym->sym.st_shndx != 0)
				ft_printf("%0*x", padding, sym->sym.st_value);
			else
				ft_printf("%*s", padding, "");
		}
		if (opt & OPT_VERBOSE)
			ft_printf(" %3d", sym->sym.st_info);
		ft_printf(" %c", sym->type);
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
			ft_printf(" %s", sym->name);
		if (opt & OPT_VERBOSE)
		{
			ft_printf(" T =");
			switch (ELF64_ST_TYPE(sym->sym.st_info))
			{
				case STT_NOTYPE:
					ft_printf(" NOTYPE");
					break ;
				case STT_OBJECT:
					ft_printf(" OBJECT");
					break ;
				case STT_FUNC:
					ft_printf(" FUNC");
					break ;
				case STT_SECTION:
					ft_printf(" SECTION");
					break ;
				case STT_FILE:
					ft_printf(" FILE");
					break ;
				case STT_LOPROC:
					ft_printf(" LOPROC");
					break ;
				case STT_HIPROC:
					ft_printf(" HIPROC");
					break ;
				case STT_COMMON:
					ft_printf(" COMMON");
					break ;
				case STT_LOOS:
					ft_printf(" LOOS");
					break ;
				case STT_HIOS:
					ft_printf(" HOS");
					break ;
			}
			ft_printf(" (%d)", ELF64_ST_TYPE(sym->sym.st_info));
			ft_printf(", B =");
			switch (ELF64_ST_BIND(sym->sym.st_info))
			{
				case STB_LOCAL:
					ft_printf(" LOCAL");
					break ;
				case STB_GLOBAL:
					ft_printf(" GLOBAL");
					break ;
				case STB_WEAK:
					ft_printf(" WEAK");
					break ;
				case STB_LOPROC:
					ft_printf(" LOPROC");
					break ;
				case STB_HIPROC:
					ft_printf(" HIPROC");
					break ;
				case STB_LOOS:
					ft_printf(" LOOS");
					break ;
				case STB_HIOS:
					ft_printf(" HIOS");
					break ;
			}
			ft_printf(" (%d)", ELF64_ST_BIND(sym->sym.st_info));
			ft_printf(", O = %d", ELF64_ST_VISIBILITY(sym->sym.st_other));
			ft_printf(", S = %d", read_uint64(sym->sym.st_size, opt));
			ft_printf(", H = %d", sym->sym.st_shndx);
			if (shndx_ok)
			{
				ft_printf(" (%s",
				ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt));
				switch (read_uint32(sheader->sh_type, opt))
				{
					case SHT_NULL:
						ft_printf(" (NULL))");
						break ;
					case SHT_PROGBITS:
						ft_printf(" (PROGBITS))");
						break ;
					case SHT_SYMTAB:
						ft_printf(" (SYMTAB))");
						break ;
					case SHT_STRTAB:
						ft_printf(" (STRTAB))");
						break ;
					case SHT_RELA:
						ft_printf(" (RELA))");
						break ;
					case SHT_HASH:
						ft_printf(" (HASH))");
						break ;
					case SHT_DYNAMIC:
						ft_printf(" (DYNAMIC))");
						break ;
					case SHT_NOTE:
						ft_printf(" (NOTE))");
						break ;
					case SHT_NOBITS:
						ft_printf(" (NOBITS))");
						break ;
					case SHT_REL:
						ft_printf(" (REL))");
						break ;
					case SHT_SHLIB:
						ft_printf(" (SHLIB))");
						break ;
					case SHT_DYNSYM:
						ft_printf(" (DYNSYM))");
						break ;
					case SHT_LOPROC:
						ft_printf(" (LOPROC))");
						break ;
					case SHT_HIPROC:
						ft_printf(" (HIPROC))");
						break ;
					case SHT_LOUSER:
						ft_printf(" (LOUSER))");
						break ;
					case SHT_HIUSER:
						ft_printf(" (HIUSER))");
						break ;
				}
				ft_printf(", flags =");
				if (read_uint64(sheader->sh_flags, opt) & SHF_WRITE)
					ft_printf(" WRITE");
				if (read_uint64(sheader->sh_flags, opt) & SHF_ALLOC)
					ft_printf(" ALLOC");
				if (read_uint64(sheader->sh_flags, opt) & SHF_EXECINSTR)
					ft_printf(" EXECINSTR");
				if (read_uint64(sheader->sh_flags, opt) & SHF_MASKPROC)
					ft_printf(" MASKPROC");
				if (read_uint64(sheader->sh_flags, opt) & SHF_MERGE)
					ft_printf(" MERGE");
				if (read_uint64(sheader->sh_flags, opt) & SHF_STRINGS)
					ft_printf(" STRINGS");
				if (read_uint64(sheader->sh_flags, opt) & SHF_INFO_LINK)
					ft_printf(" INFO_LINK");
				if (read_uint64(sheader->sh_flags, opt) & SHF_LINK_ORDER)
					ft_printf(" LINK_ORDER");
				if (read_uint64(sheader->sh_flags, opt) & SHF_OS_NONCONFORMING)
					ft_printf(" OS_NONCONFORMING");
				if (read_uint64(sheader->sh_flags, opt) & SHF_GROUP)
					ft_printf(" GROUP");
				if (read_uint64(sheader->sh_flags, opt) & SHF_TLS)
					ft_printf(" TLS");
				if (read_uint64(sheader->sh_flags, opt) & SHF_MASKOS)
					ft_printf(" MASKOS");
				if (read_uint64(sheader->sh_flags, opt) & SHF_MIPS_MERGE)
					ft_printf(" MIPS_MERGE");
				if (read_uint64(sheader->sh_flags, opt) & SHF_ORDERED)
					ft_printf(" ORDERED");
				if (read_uint64(sheader->sh_flags, opt) & SHF_EXCLUDE)
					ft_printf(" EXCLUDE");
				ft_printf(" (%d)", read_uint64(sheader->sh_flags, opt));
			}
			else
			{
				switch (sym->sym.st_shndx)
				{
					case SHN_UNDEF:
					ft_printf(" (UNDEF)");
						break ;
					case SHN_ABS:
						ft_printf(" (ABS)");
						break ;
					case SHN_COMMON:
						ft_printf(" (COMMON)");
						break ;
				}
			}
		}
		ft_printf("\n");
		lst = lst->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 16:05:36 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 12:37:11 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "options.h"

/*
**	Frees the content of a t_sym
*/

void			delsym(void *file, size_t size)
{
	(void)size;
	(void)file;
}

void	set_symbol_type(t_sym *sym, char *ptr, Elf64_Ehdr *header,
Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt)
{
	Elf64_Shdr *sheader = (Elf64_Shdr*) (ptr + header->e_shoff
		+ (header->e_shentsize * sym->sym->st_shndx));
	if (opt & OPT_VERBOSE)
	{
		ft_printf("------------------------------------------------\n");
		ft_printf("\tName = %u (%s)\n", (uint32_t)sym->sym->st_name,
		ptr + shstr->sh_offset + sym->sym->st_name);
		ft_printf("\tInfo = %d\n", sym->sym->st_info);
		ft_printf("\tOther = %d\n", sym->sym->st_other);
		ft_printf("\tSection = %hu (%s)\n", (uint16_t)sym->sym->st_shndx,
		ptr + shstrhdr->sh_offset + sheader->sh_name);
		ft_printf("\tValue = %016x\n", sym->sym->st_value);
		ft_printf("\tSize = %lu\n", (uint64_t)sym->sym->st_size);
	}
	if (sym->sym->st_value == 0)
		sym->type = 'u';
	switch (sym->sym->st_info)
	{
		case 0:
			sym->type = 'r';
			break ;
		case 1:
			sym->type = 'd';
			break ;
		case 2:
			sym->type = 't';
			break ;
		case 16:
			sym->type = 'b';
			break ;
		case 17:
			sym->type = 'd';
			break ;
		case 18:
			sym->type = 'T';
			break ;
		case 32:
			sym->type = 'w';
			break ;
		case 34:
			sym->type = 'w';
			break ;
		default:
			sym->type = '?';
			break ;
	}
	if (ft_strequ(ptr + shstrhdr->sh_offset + sheader->sh_name, ".rodata"))
		sym->type = 'r';
	else if (ft_strequ(ptr + shstrhdr->sh_offset + sheader->sh_name, ".eh_frame"))
		sym->type = 'r';
	else if (ft_strequ(ptr + shstrhdr->sh_offset + sheader->sh_name, ".data"))
	{
		sym->type = 'd';
		if (ELF64_ST_BIND(sym->sym->st_info) == 2)
			sym->type = 'W';
	}
	else if (ft_strequ(ptr + shstrhdr->sh_offset + sheader->sh_name, ".bss"))
		sym->type = 'b';
	else if (ft_strequ(ptr + shstrhdr->sh_offset + sheader->sh_name, ".init_array"))
		sym->type = 'd';
	if (ELF64_ST_BIND(sym->sym->st_info) == 1)
		sym->type = ft_toupper(sym->type);
}

void	print_symbols(t_dlist *lst, int opt)
{
	t_sym		*sym;

	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		sym = (t_sym*)lst->content;
		if (sym->sym->st_value != 0)
			ft_printf("%016x", sym->sym->st_value);
		else
			ft_printf("%16s", "");
		if (opt & OPT_VERBOSE)
			ft_printf(" %d", sym->sym->st_info);
		if (sym->sym->st_value == 0 && sym->sym->st_info == 18)
			ft_printf(" U");
		else
			ft_printf(" %c", sym->type);
		ft_printf(" %s", sym->name);
		if (opt & OPT_VERBOSE)
		{
			ft_printf("  T = %d", ELF64_ST_TYPE(sym->sym->st_info));
			ft_printf(", B = %d", ELF64_ST_BIND(sym->sym->st_info));
			ft_printf(", O = %d", ELF64_ST_VISIBILITY(sym->sym->st_other));
			ft_printf(", S = %d", sym->sym->st_size);
			ft_printf(", H = %d", sym->sym->st_shndx);
		}
		ft_printf("\n");
		lst = lst->next;
	}
}

void	handle_64(char *ptr, int opt)
{
	Elf64_Ehdr	*header;
	Elf64_Shdr	*sheader;
	Elf64_Shdr	*shstrhdr;
	Elf64_Shdr	*shstr;
	Elf64_Off	i;
	uint64_t	j;
	Elf64_Sym	*elf_sym;
	t_dlist		*lst;
	t_dlist		*new;
	t_sym		sym;

	header = (Elf64_Ehdr*) ptr;
	if (opt & OPT_VERBOSE)
	{
		ft_printf("%hu program entries\n", (uint16_t)header->e_phnum);
		ft_printf("%hu sections entries\n", (uint16_t)header->e_shnum);
		ft_printf("Offset = %lu ", (Elf64_Off)header->e_shoff);
		ft_printf("Size = %hu\n", (uint16_t)header->e_shentsize);
		ft_printf("Strings index = %hu\n", (uint16_t)header->e_shstrndx);
	}
	shstrhdr = (Elf64_Shdr*)(ptr + header->e_shoff + (header->e_shentsize * header->e_shstrndx));
	lst = NULL;
	new = NULL;
	sym.sym = NULL;
	sym.name = NULL;
	i = 0;
	if (opt & OPT_VERBOSE)
	{
		ft_printf("Section types:\n");
		ft_printf("SHT_NULL = %d\n", SHT_NULL);
		ft_printf("SHT_PROGBITS = %d\n", SHT_PROGBITS);
		ft_printf("SHT_SYMTAB = %d\n", SHT_SYMTAB);
		ft_printf("SHT_STRTAB = %d\n", SHT_STRTAB);
		ft_printf("SHT_RELA = %d\n", SHT_RELA);
		ft_printf("SHT_HASH = %d\n", SHT_HASH);
		ft_printf("SHT_DYNAMIC = %d\n", SHT_DYNAMIC);
		ft_printf("SHT_NOTE = %d\n", SHT_NOTE);
		ft_printf("SHT_REL = %d\n", SHT_REL);
		ft_printf("SHT_SHLIB = %d\n", SHT_SHLIB);
		ft_printf("SHT_DYNSYM = %d\n", SHT_DYNSYM);
		ft_printf("SHT_LOPROC = %d\n", SHT_LOPROC);
		ft_printf("SHT_HIPROC = %d\n", SHT_HIPROC);
		ft_printf("SHT_LOUSER = %d\n", SHT_LOUSER);
		ft_printf("SHT_HIUSER = %d\n", SHT_HIUSER);
	}
	while (i < header->e_shnum)
	{
		sheader = (Elf64_Shdr*) (ptr + header->e_shoff
		+ (header->e_shentsize * i));
		if (opt & OPT_VERBOSE)
		{
			ft_printf("------------------------------\n");
			ft_printf("[Section %d]\n", i);
			ft_printf("Section name = %u (%s)\n", (uint32_t)sheader->sh_name,
			ptr + shstrhdr->sh_offset + sheader->sh_name);
			ft_printf("Section type = %u\n", (uint32_t)sheader->sh_type);
			ft_printf("Section flags = %u\n", (uint32_t)sheader->sh_flags);
			ft_printf("Section size = %lu\n", (uint64_t)sheader->sh_size);
			ft_printf("Section Link = %u\n", (uint32_t)sheader->sh_link);
			ft_printf("Section offset = %lu\n", (uint64_t)sheader->sh_offset);
			ft_printf("Entry size = %lu\n", (uint64_t)sheader->sh_entsize);
		}
		if (sheader->sh_type == SHT_SYMTAB)
		{
			j = 0;
			if (opt & OPT_VERBOSE)
				ft_printf("Symbol section has %d symbols\n",
				sheader->sh_size / sheader->sh_entsize);
			shstr = (Elf64_Shdr*)(ptr + header->e_shoff + (header->e_shentsize * sheader->sh_link));
			while (j < sheader->sh_size / sheader->sh_entsize)
			{
				ft_bzero(&sym, sizeof(sym));
				elf_sym = (Elf64_Sym *)(ptr + sheader->sh_offset + (sheader->sh_entsize * j));
				if (elf_sym->st_info == STT_FILE || elf_sym->st_info == STT_SECTION
					|| (elf_sym->st_info == 0 && elf_sym->st_value == 0))
				{
					j++;
					continue;
				}
				sym.sym = elf_sym;
				sym.name = ptr + shstr->sh_offset + elf_sym->st_name;
				set_symbol_type(&sym, ptr, header, shstr, shstrhdr, opt);
				if (!(new = ft_dlstnew(&sym, sizeof(sym))))
				{
					custom_error("ft_lstnew:");
					ft_dlstdelfront(&lst, delsym);
					return ;
				}
				ft_dlstinsert(&lst, new, compare_names);
				j++;
			}
		}
		else if (sheader->sh_type == SHT_STRTAB && opt & OPT_VERBOSE)
		{
			ft_printf("{cyan}String table{reset}\n");
		}
		i++;
	}
	print_symbols(lst, opt);
	ft_dlstdelfront(&lst, delsym);
}
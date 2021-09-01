/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 16:05:36 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/06 16:05:10 by lnicosia         ###   ########.fr       */
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

void	print_symbols(t_dlist *lst, int opt)
{
	t_sym		*sym;
	static char	types[64] = {'?'};

	types[0] = 'r';
	types[1] = 'd';
	types[2] = 't';
	types[16] = 'B';
	types[17] = 'D';
	types[18] = 'U';
	types[32] = 'w';
	types[34] = 'w';
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
		else
			ft_printf(" %c", types[sym->sym->st_info]);
		ft_printf(" %s\n", sym->name);
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
	(void)shstrhdr;
	shstrhdr = (Elf64_Shdr*)(ptr + header->e_shoff + (header->e_shentsize * header->e_shstrndx));
	lst = NULL;
	new = NULL;
	sym.sym = NULL;
	sym.name = NULL;
	i = 0;
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
				elf_sym = (Elf64_Sym *)(ptr + sheader->sh_offset + (sheader->sh_entsize * j));
				if (opt & OPT_VERBOSE)
				{
					ft_printf("------------------------------------------------\n");
					ft_printf("\tName = %u (%s)\n", (uint32_t)elf_sym->st_name,
					ptr + shstr->sh_offset + elf_sym->st_name);
					ft_printf("\tInfo = %d\n", elf_sym->st_info);
					ft_printf("\tOther = %d\n", elf_sym->st_other);
					ft_printf("\tIndex = %hu\n", (uint16_t)elf_sym->st_shndx);
					ft_printf("\tValue = %016x\n", elf_sym->st_value);
					ft_printf("\tSize = %lu\n", (uint64_t)elf_sym->st_size);
				}
				if (elf_sym->st_info == STT_FILE || elf_sym->st_info == STT_SECTION
					|| (elf_sym->st_info == 0 && elf_sym->st_value == 0))
				{
					j++;
					continue;
				}
				sym.sym = elf_sym;
				sym.name = ptr + shstr->sh_offset + elf_sym->st_name;
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
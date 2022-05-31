/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
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

/*
**	Frees the content of a t_sym
*/

static void			delsym(void *sym, size_t size)
{
	(void)size;
	ft_strdel(&((t_sym64*)sym)->name);
}

static void	set_symbol_type(t_sym64 *sym, char *ptr, Elf64_Ehdr *header,
Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt)
{
	int			shndx_ok;

	if (sym->sym.st_shndx >= read_uint16(header->e_shnum, opt))
		shndx_ok = 0;
	else
		shndx_ok = 1;
	Elf64_Shdr *sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * sym->sym.st_shndx));
	if (opt & OPT_VERBOSE)
	{
		ft_bprintf(0, "------------------------------------------------\n");
		ft_bprintf(0, "\tName = %u (%s)\n", read_uint32(sym->sym.st_name, opt),
		ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt));
		ft_bprintf(0, "\tInfo = %d\n", sym->sym.st_info);
		ft_bprintf(0, "\tOther = %d\n", sym->sym.st_other);
		ft_bprintf(0, "\tSection = %hu", sym->sym.st_shndx);
		if (shndx_ok)
		{
			ft_bprintf(0, " (%s)", ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
			+ read_uint32(sheader->sh_name, opt));
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
				default:
					ft_bprintf(0, " (?)");
					break ;
			}
		}
		ft_bprintf(0, "\n\tValue = %016x\n", sym->sym.st_value);
		ft_bprintf(0, "\tSize = %lu\n", sym->sym.st_size);
	}
	if (shndx_ok)
	{
		if (read_uint32(sheader->sh_type, opt) == SHT_NOBITS)
			sym->type = 'b';
		else if (!(read_uint64(sheader->sh_flags, opt) & SHF_WRITE))
		{
			if (read_uint64(sheader->sh_flags, opt) & SHF_ALLOC
				&& read_uint64(sheader->sh_flags, opt) & SHF_EXECINSTR)
				sym->type = 't';
			else
				sym->type = 'r';
		}
		else
			sym->type = 'd';
	}
	else if (sym->sym.st_shndx == SHN_COMMON)
			sym->type = 'c';
	switch (ELF64_ST_TYPE(sym->sym.st_info))
	{
		case STT_FUNC:
			if (sym->sym.st_shndx == 0)
				sym->type = 'u';
			else
				sym->type = 't';
			break ;
		case STT_LOOS:
			sym->type = 'i';
			break ;
		case STT_SECTION:
			if (read_uint64(sheader->sh_flags, opt) == 0)
				sym->type = 'n';
			break ;
	}
	if (sym->sym.st_value == 0
		&& sym->type != 'b' && sym->type != 'n'
		&& shndx_ok && read_uint32(sheader->sh_type, opt) != SHT_NOTE
		&& read_uint64(sheader->sh_flags, opt) == 0)
		sym->type = 'u';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".data")
		|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".tm_clone_table")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), "preinit_array")))
		sym->type = 'd';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".rodata")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".gnu.offload")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".exit.data")))
		sym->type = 'r';
	if (shndx_ok && ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".eh_frame"))
	{
		if (read_uint32(sheader->sh_type, opt) == SHT_NOBITS)
			sym->type = 'b';
		else if (read_uint32(sheader->sh_type, opt) == SHT_PROGBITS
			&& read_uint64(sheader->sh_flags, opt) == (SHF_ALLOC | SHF_WRITE))
			sym->type = 'd';
		else
			sym->type = 'r';
	}
	if (shndx_ok && sym->type != 'i'
		&& (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".text")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".init")
		|| ft_strbegin(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".plt")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), "_freeres_fn")
		|| (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".note")
			&& read_uint64(sheader->sh_flags, opt) & SHF_EXECINSTR)
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".fini")))
		sym->type = 't';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), "bss")))
		sym->type = 'b';
	if (shndx_ok && read_uint64(sheader->sh_flags, opt) == (SHF_MERGE | SHF_STRINGS))
		sym->type = 'n';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), "warning")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".group")
		|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".ARM.attributes")))
		sym->type = 'n';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".debug")
		|| ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".zdebug")))
		sym->type = 'N';
	if ((!shndx_ok && sym->sym.st_shndx == SHN_ABS)
		|| (shndx_ok &&
			(ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".shstrtab")
			|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".strtab")
			|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".symtab"))))
		sym->type = 'a';
	switch (ELF64_ST_BIND(sym->sym.st_info))
	{
		case STB_WEAK:
			if (ELF64_ST_TYPE(sym->sym.st_info) == STT_OBJECT)
			{
				if (sym->sym.st_size == 0)
					sym->type = 'v';
				else
					sym->type = 'V';
			}
			else if (ELF64_ST_TYPE(sym->sym.st_info) != STT_LOOS)
			{
				sym->type = 'w';
				if (shndx_ok && read_uint64(sheader->sh_flags, opt) != 0)
					sym->type = 'W';
			}
			break ;
		case STB_GLOBAL:
			if (sym->type != 'w' && sym->type != 'i')
				sym->type = ft_toupper(sym->type);
			break ;
		case STB_LOOS:
			sym->type = 'u';
			break ;
	}
}

static int	search_for_duplicates(t_dlist *lst, char *name)
{
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		t_sym64 *sym = (t_sym64*)lst->content;
		if (ft_strequ(sym->name, name))
			return (1);
		lst = lst->next;
	}
	return (0);
}

// Searching for symbol names in flto sections

static int	search_for_lto_symbol_section(char *ptr, char *name, Elf64_Shdr *shstrhdr, int opt)
{
	char	*str;

	if (!(str = ft_strjoin(".gnu.lto_", name)))
		return (0);
	Elf64_Ehdr *header = (Elf64_Ehdr*)ptr;
	uint64_t	i = 0;
	while (i < read_uint16(header->e_shnum, opt))
	{
		Elf64_Shdr *sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * i));
		char *section_name =
		ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
		+ read_uint32(sheader->sh_name, opt);
		// If the section name's contain our string then a '.',
		// we found what we wanted
		if (ft_strstr(section_name, str) && *(section_name + ft_strlen(str)) == '.')
			return (1);
		i++;
	}
	return (0);
}

void	handle_64(char *file, char *ptr, long int file_size, int opt)
{
	Elf64_Ehdr	*header;
	Elf64_Shdr	*sheader;
	Elf64_Shdr	*shstrhdr;
	Elf64_Shdr	*shstr;
	Elf64_Off	i;
	uint64_t	j;
	uint64_t	sym_count;
	Elf64_Sym	*elf_sym;
	t_dlist		*lst;
	t_dlist		*new;
	t_sym64		sym;
	char		*str;
	int			(*compare)(void *, void*);

	lst = NULL;
	new = NULL;
	sym.name = NULL;
	sym_count = 0;
	shstr = NULL;
	header = (Elf64_Ehdr*)ptr;
	compare = set_compare_func(64, opt);
	if (opt & OPT_VERBOSE)
	{
		switch (read_uint16(header->e_type, opt))
		{
			case ET_NONE:
				ft_bprintf(0, "Unknown type\n");
				break;
			case ET_REL:
				ft_bprintf(0, "Relocatable file\n");
				break;
			case ET_EXEC:
				ft_bprintf(0, "Executable file\n");
				break;
			case ET_DYN:
				ft_bprintf(0, "Shared object\n");
				break;
			case ET_CORE:
				ft_bprintf(0, "Core file\n");
				break;
		}
		ft_bprintf(0, "%hu program entries\n", read_uint16(header->e_phnum, opt));
		ft_bprintf(0, "%hu sections entries\n", read_uint16(header->e_shnum, opt));
		ft_bprintf(0, "Section headers offset = %lu\n", read_long_unsigned_int(header->e_shoff, opt));
		ft_bprintf(0, "Strings section header index = %hu\n", read_uint64(header->e_shstrndx, opt));
		ft_bprintf(0, "Sections header's size = %hu (total size = %d)\n", read_uint16(header->e_shentsize, opt),
		read_uint16(header->e_shentsize, opt) * read_uint16(header->e_shnum, opt));
		ft_bprintf(0, "Program header's size = %hu (total size = %d)\n",
		read_uint16(header->e_phentsize, opt), read_uint16(header->e_phentsize, opt) * read_uint16(header->e_phnum, opt));
			ft_bprintf(0, "Endian = ");
			if (ptr[5] == ELFDATA2LSB)
				ft_bprintf(0, "little\n");
			else if (ptr[5] == ELFDATA2MSB)
				ft_bprintf(0, "big\n");

	}
	// Check if the file is big enough to contain all the section headers
	if ((long int)read_long_unsigned_int(header->e_shoff, opt)
		+ read_uint16(header->e_shentsize, opt) * read_uint16(header->e_shnum, opt) > file_size
		|| (long int)read_long_unsigned_int(header->e_phoff, opt)
		+ read_uint16(header->e_phentsize, opt) * read_uint16(header->e_phnum, opt) > file_size)
	{
		ft_bprintf(0, "ft_nm: %s: file too short\n", file);
		custom_error("ft_nm: %s: File truncated\n", file);
		return ;
	}
	shstrhdr = (Elf64_Shdr*)(ptr + read_long_unsigned_int(header->e_shoff, opt)
	+ (read_uint16(header->e_shentsize, opt) * read_uint16(header->e_shstrndx, opt)));
	if (read_uint32(shstrhdr->sh_type, opt) != SHT_NOBITS
		&& (long int)read_long_unsigned_int(shstrhdr->sh_offset, opt)
		+ (long int)read_uint64(shstrhdr->sh_size, opt) > file_size)
	{
		ft_bprintf(0, "ft:nm: %s: file too short\n", file);
		custom_error("ft_nm: %s: File format not recognized\n", file);
		ft_dlstdelfront(&lst, delsym);
		return ;
	}
	i = 0;
	while (i < read_uint16(header->e_shnum, opt))
	{
		sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * i));
		// Check if the file is big enough to contain the section
		if (read_uint32(sheader->sh_type, opt) != SHT_NOBITS
			&& (long int)read_long_unsigned_int(sheader->sh_offset, opt)
			+ (long int)read_uint64(sheader->sh_size, opt) > file_size)
		{
			ft_bprintf(0, "ft:nm: %s: file too short\n", file);
			custom_error("ft_nm: %s: File truncated\n", file);
			ft_dlstdelfront(&lst, delsym);
			return ;
		}
		if (opt & OPT_VERBOSE)
		{
			ft_bprintf(0, "------------------------------\n");
			ft_bprintf(0, "[Section %d]\n", i);
			ft_bprintf(0, "Section name = %u (%s)\n", read_uint32(sheader->sh_name, opt),
			ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt));
			ft_bprintf(0, "Section type = %u", read_uint32(sheader->sh_type, opt));
			switch (read_uint32(sheader->sh_type, opt))
			{
				case SHT_NULL:
					ft_bprintf(0, " (NULL)");
					break ;
				case SHT_PROGBITS:
					ft_bprintf(0, " (PROGBITS)");
					break ;
				case SHT_SYMTAB:
					ft_bprintf(0, " (SYMTAB)");
					break ;
				case SHT_STRTAB:
					ft_bprintf(0, " (STRTAB)");
					break ;
				case SHT_RELA:
					ft_bprintf(0, " (RELA)");
					break ;
				case SHT_HASH:
					ft_bprintf(0, " (HASH)");
					break ;
				case SHT_DYNAMIC:
					ft_bprintf(0, " (DYNAMIC)");
					break ;
				case SHT_NOTE:
					ft_bprintf(0, " (NOTE)");
					break ;
				case SHT_NOBITS:
					ft_bprintf(0, " (NOBITS)");
					break ;
				case SHT_REL:
					ft_bprintf(0, " (REL)");
					break ;
				case SHT_SHLIB:
					ft_bprintf(0, " (SHLIB)");
					break ;
				case SHT_DYNSYM:
					ft_bprintf(0, " (DYNSYM)");
					break ;
				case SHT_LOPROC:
					ft_bprintf(0, " (LOPROC)");
					break ;
				case SHT_HIPROC:
					ft_bprintf(0, " (HIPROC)");
					break ;
				case SHT_LOUSER:
					ft_bprintf(0, " (LOUSER)");
					break ;
				case SHT_HIUSER:
					ft_bprintf(0, " (HIUSER)");
					break ;
			}
			ft_bprintf(0, "\nSection flags = %lu\n", read_uint64(sheader->sh_flags, opt));
			ft_bprintf(0, "Section size = %lu\n", read_uint64(sheader->sh_size, opt));
			ft_bprintf(0, "Section link = %u\n", read_uint32(sheader->sh_link, opt));
			ft_bprintf(0, "Section info = %u\n", read_uint32(sheader->sh_info, opt));
			ft_bprintf(0, "Section offset = %lu\n", read_long_unsigned_int(sheader->sh_offset, opt));
			ft_bprintf(0, "Entry size = %lu\n", read_uint64(sheader->sh_entsize, opt));
		}
		if (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
			+ read_uint32(sheader->sh_name, opt), ".gnu.lto"))
			opt |= OPT_LTO;
		if (read_uint32(sheader->sh_type, opt) == SHT_SYMTAB)
		{
			sym_count += read_uint64(sheader->sh_size, opt) / read_uint64(sheader->sh_entsize, opt);
			j = 0;
			if (opt & OPT_VERBOSE)
				ft_bprintf(0, "Symbol section has %d symbols\n",
				read_uint64(sheader->sh_size, opt) / read_uint64(sheader->sh_entsize, opt));
			shstr = (Elf64_Shdr*)(ptr + read_long_unsigned_int(header->e_shoff, opt)
			+ (read_uint16(header->e_shentsize, opt) * read_uint32(sheader->sh_link, opt)));
			while (j < read_uint64(sheader->sh_size, opt) / read_uint64(sheader->sh_entsize, opt))
			{
				ft_bzero(&sym, sizeof(sym));
				elf_sym = (Elf64_Sym *)(ptr + read_long_unsigned_int(sheader->sh_offset, opt)
				+ (read_uint64(sheader->sh_entsize, opt) * j));
				Elf64_Shdr *shdr = NULL;
				if (read_uint16(elf_sym->st_shndx, opt) < read_uint16(header->e_shnum, opt))
					shdr = (Elf64_Shdr*)(ptr + read_long_unsigned_int(header->e_shoff, opt)
					+ (read_uint16(header->e_shentsize, opt) * read_uint16(elf_sym->st_shndx, opt)));
				if ((!(opt & OPT_A && !(opt & OPT_LTO))
					&& ((!shdr && read_uint16(elf_sym->st_shndx, opt) != SHN_ABS
					&& read_uint16(elf_sym->st_shndx, opt) != SHN_COMMON) || elf_sym->st_info == STT_FILE
					|| elf_sym->st_info == STT_SECTION
					|| (shdr && read_uint64(shdr->sh_flags, opt) & SHF_MASKPROC)
					|| (opt & OPT_LTO &&
						(read_uint16(elf_sym->st_shndx, opt) == SHN_COMMON
						|| (read_uint16(header->e_type, opt) == ET_REL
						&& ft_strequ("_GLOBAL_OFFSET_TABLE_",
						ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(elf_sym->st_name, opt)))
						|| (ELF64_ST_TYPE(elf_sym->st_info) == STT_FUNC && ELF64_ST_BIND(elf_sym->st_info) == STB_LOCAL
						&& read_long_unsigned_int(elf_sym->st_value, opt) == 0
						&& ft_strequ(".text.unlikely", ptr
						+ read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(shdr->sh_name, opt)))))))
					|| (elf_sym->st_info == 0 && read_long_unsigned_int(elf_sym->st_value, opt) == 0
					&& read_uint64(elf_sym->st_size, opt) == 0
					&& read_uint32(elf_sym->st_name, opt) == 0 && read_uint16(elf_sym->st_shndx, opt) == 0))
				{
					j++;
					continue;
				}
				sym.sym = *elf_sym;
				sym.type = 0;
				sym.name = ptr + read_long_unsigned_int(shstr->sh_offset, opt)
				+ read_uint32(elf_sym->st_name, opt);
				sym.sym.st_value = read_uint64(elf_sym->st_value, opt);
				sym.sym.st_shndx = read_uint16(elf_sym->st_shndx, opt);
				sym.sym.st_size = read_uint64(elf_sym->st_size, opt);
				if (opt & OPT_SIZE_SORT && ELF64_ST_TYPE(elf_sym->st_info) == STT_SECTION
					&& (read_uint64(shdr->sh_flags, opt) != (SHF_ALLOC | SHF_WRITE)))
				{
					sym.sym.st_size = read_uint64(shdr->sh_size, opt);
				}
				if (opt & OPT_A && ELF64_ST_TYPE(elf_sym->st_info) == STT_SECTION)
				{
					sym.name = ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
					+ read_uint32(shdr->sh_name, opt);
				}
				size_t k = 0;
				if (opt & OPT_D)
				{
					while (sym.name[k] && sym.name[k] != '@')
						k++;
				}
				else
					k = ft_strlen(sym.name);
				if (!(sym.name = ft_strsub(sym.name, 0, k)))
				{
					ft_dlstdelfront(&lst, delsym);
					return ;
				}
				if (opt & OPT_LTO && search_for_duplicates(lst, sym.name))
				{
					j++;
					continue ;
				}
				if (opt & OPT_VERBOSE)
				{
					ft_bprintf(0, "------------------------------------------------\n");
					ft_bprintf(0, "\t\tSymbol %d\n", j);
				}
				set_symbol_type(&sym, ptr, header, shstr, shstrhdr, opt);
				if (sym.type == 'C')
					sym.sym.st_value = sym.sym.st_size;
				if (!(new = ft_dlstnew(&sym, sizeof(sym))))
				{
					custom_error("ft_lstnew:");
					ft_dlstdelfront(&lst, delsym);
					return ;
				}
				ft_dlstinsert(&lst, new, compare);
				j++;
			}
		}
		// Specific case for files optimized with -flto
		// Each symbol name is in the ".gnu.lto_symtab" section
		// as raw data
		if (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
			+ read_uint32(sheader->sh_name, opt), ".gnu.lto_.symtab"))
		{
			if (opt & OPT_VERBOSE)
				ft_bprintf(0, "{yellow}Symbol section from -flto optimization{reset}\n");
			j = 0;
			// Let's check the section for strings
			str = ptr + read_long_unsigned_int(sheader->sh_offset, opt);
			while (j < read_uint64(sheader->sh_size, opt))
			{
				if (ft_isprint(*(str + j)))
				{
					ft_bzero(&sym.sym.st_name, sizeof(Elf64_Sym)); 
					if (!(sym.name = ft_strdup(str + j)))
					{
						ft_dlstdelfront(&lst, delsym);
						return ;
					}
					// Eliminate strings with non printable characters
					// and that already exist
					uint64_t k = j;
					int valid = 1;
					while (*(str + k))
					{
						if (!ft_isprint(*(str + k)))
						{
							valid = 0;
							break ;
						}
						k++;
					}
					if (valid == 0 || search_for_duplicates(lst, sym.name))
					{
						j += ft_strlen(str + j);
						continue ;
					}
					sym_count++;
					// If a section named ".gnu.lto_{symbol_name}" exists,
					// the symbol is of type T and has an adress of 00000000
					if (search_for_lto_symbol_section(ptr, sym.name, shstrhdr, opt))
					{
						sym.sym.st_shndx = (Elf64_Section)i;
						sym.type = 'T';
					}
					// Otherwise, it's an undefined symbol
					else
					{
						sym.sym.st_shndx = 0;
						sym.type = 'U';
					}
					if (opt & OPT_VERBOSE)
					{
						ft_bprintf(0, "%s ", (str + j));
					}
					j += ft_strlen(str + j);
					if (!(new = ft_dlstnew(&sym, sizeof(sym))))
					{
						custom_error("ft_lstnew:");
						ft_dlstdelfront(&lst, delsym);
						return ;
					}
					ft_dlstinsert(&lst, new, compare);
				}
				j++;
			}
			if (opt & OPT_VERBOSE)
				ft_bprintf(0, "\n");
		}
		i++;
	}
	if (opt & OPT_PRINT_FILE_NAME)
	{
		if (!(opt & OPT_POSIX))
			ft_bprintf(0, "\n");
		ft_bprintf(0, "%s:\n", file);
	}
	if (opt & OPT_BSD)
		print_64_bsd_symbols(lst, file, ptr, header, shstr, shstrhdr, opt);
	else if (opt & OPT_SYSV)
		print_64_sysv_symbols(lst, file, ptr, header, shstr, shstrhdr, opt);
	else if (opt & OPT_POSIX)
		print_64_posix_symbols(lst, file, ptr, header, shstr, shstrhdr, opt);
	ft_dlstdelfront(&lst, delsym);
	if (sym_count == 0)
		custom_error("ft_nm: %s: no symbols\n", file);
}

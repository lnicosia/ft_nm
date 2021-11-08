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

static void			delsym(void *file, size_t size)
{
	(void)size;
	(void)file;
}

static void	set_symbol_type(t_sym64 *sym, char *ptr, Elf64_Ehdr *header,
Elf64_Shdr *shstr, Elf64_Shdr *shstrhdr, int opt)
{
	int			shndx_ok;

	if (read_uint16(sym->sym.st_shndx, opt) >= read_uint16(header->e_shnum, opt))
		shndx_ok = 0;
	else
		shndx_ok = 1;
	Elf64_Shdr *sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * read_uint16(sym->sym.st_shndx, opt)));
	if (opt & OPT_VERBOSE)
	{
		ft_printf("------------------------------------------------\n");
		ft_printf("\tName = %u (%s)\n", read_uint32(sym->sym.st_name, opt),
		ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt));
		ft_printf("\tInfo = %d\n", sym->sym.st_info);
		ft_printf("\tOther = %d\n", sym->sym.st_other);
		ft_printf("\tSection = %hu", read_uint16(sym->sym.st_shndx, opt));
		if (shndx_ok)
			ft_printf(" (%s)\n", ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
			+ read_uint32(sheader->sh_name, opt));
		else
		{
			switch (read_uint16(sym->sym.st_shndx, opt))
			{
				case SHN_UNDEF:
					ft_printf(" (UNDEF)\n");
					break ;
				case SHN_ABS:
					ft_printf(" (ABS)\n");
					break ;
				case SHN_COMMON:
					ft_printf(" (COMMON)\n");
					break ;
			}
		}
		ft_printf("\tValue = %016x\n", read_long_unsigned_int(sym->sym.st_value, opt));
		ft_printf("\tSize = %lu\n", read_uint64(sym->sym.st_size, opt));
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
	else if (read_uint16(sym->sym.st_shndx, opt) == SHN_COMMON)
			sym->type = 'c';
	switch (ELF64_ST_TYPE(sym->sym.st_info))
	{
		case STT_FUNC:
			if (read_uint16(sym->sym.st_shndx, opt) == 0)
				sym->type = 'u';
			else
				sym->type = 't';
			break ;
		case STT_LOOS:
			sym->type = 'i';
			break ;
	}
	if (read_long_unsigned_int(sym->sym.st_value, opt) == 0 && sym->type != 'b'
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
		|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".init_array")
		|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".fini_array")
		|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".init.rodata")
		|| ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".init.data")))
		sym->type = 't';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), "bss")))
		sym->type = 'b';
	if (!shndx_ok && read_uint16(sym->sym.st_shndx, opt) == SHN_ABS)
		sym->type = 'a';
	if (shndx_ok && read_uint64(sheader->sh_flags, opt) == (SHF_MERGE | SHF_STRINGS))
		sym->type = 'n';
	if (shndx_ok && (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), "warning")
		|| (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".group"))
		|| (ft_strequ(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".ARM.attributes"))))
		sym->type = 'n';
	if (shndx_ok && ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt), ".debug"))
		sym->type = 'N';
	switch (ELF64_ST_BIND(sym->sym.st_info))
	{
		case STB_WEAK:
			if (ELF64_ST_TYPE(sym->sym.st_info) == STT_OBJECT)
			{
				if (read_uint64(sym->sym.st_size, opt) == 0)
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

static void	print_symbols(t_dlist *lst, char *file, char *ptr, Elf64_Ehdr *header,
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
		if (read_uint16(sym->sym.st_shndx, opt) >= read_uint16(header->e_shnum, opt))
			shndx_ok = 0;
		else
			shndx_ok = 1;
		Elf64_Shdr	*sheader = (Elf64_Shdr*) (ptr + read_long_unsigned_int(header->e_shoff, opt)
		+ (read_uint16(header->e_shentsize, opt) * read_uint16(sym->sym.st_shndx, opt)));
		if (opt & OPT_O)
			ft_printf("%s:", file);
		if (sym->type == 'C')
			ft_printf("%0*x", padding, read_uint64(sym->sym.st_size, opt));
		else if (ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vclock_page")
			|| ft_strstr(ptr + read_long_unsigned_int(shstr->sh_offset, opt) + read_uint32(sym->sym.st_name, opt), "vvar_"))
			ft_printf("%*x%0*x", padding / 2, 0xffffffff, padding / 2, read_long_unsigned_int(sym->sym.st_value, opt));
		else
		{
			if (read_uint16(sym->sym.st_shndx, opt) != 0)
				ft_printf("%0*x", padding, read_long_unsigned_int(sym->sym.st_value, opt));
			else
				ft_printf("%*s", padding, "");
		}
		if (opt & OPT_VERBOSE)
			ft_printf(" %3d", sym->sym.st_info);
		ft_printf(" %c", sym->type);
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
			ft_printf(", H = %d", read_uint16(sym->sym.st_shndx, opt));
			if (shndx_ok)
			{
				ft_printf(" (%s), flags =", ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt));
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
				switch (read_uint16(sym->sym.st_shndx, opt))
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

	lst = NULL;
	new = NULL;
	sym.name = NULL;
	sym_count = 0;
	shstr = NULL;
	header = (Elf64_Ehdr*)ptr;
	if (opt & OPT_VERBOSE)
	{
		switch (read_uint16(header->e_type, opt))
		{
			case ET_NONE:
				ft_printf("Unknown type\n");
				break;
			case ET_REL:
				ft_printf("Relocatable file\n");
				break;
			case ET_EXEC:
				ft_printf("Executable file\n");
				break;
			case ET_DYN:
				ft_printf("Shared object\n");
				break;
			case ET_CORE:
				ft_printf("Core file\n");
				break;
		}
		ft_printf("%hu program entries\n", read_uint16(header->e_phnum, opt));
		ft_printf("%hu sections entries\n", read_uint16(header->e_shnum, opt));
		ft_printf("Section headers offset = %lu\n", read_long_unsigned_int(header->e_shoff, opt));
		ft_printf("Strings section header index = %hu\n", read_uint64(header->e_shstrndx, opt));
		ft_printf("Sections header's size = %hu (total size = %d)\n", read_uint16(header->e_shentsize, opt),
		read_uint16(header->e_shentsize, opt) * read_uint16(header->e_shnum, opt));
		ft_printf("Program header's size = %hu (total size = %d)\n",
		read_uint16(header->e_phentsize, opt), read_uint16(header->e_phentsize, opt) * read_uint16(header->e_phnum, opt));
			ft_printf("Endian = ");
			if (ptr[5] == ELFDATA2LSB)
				ft_printf("little\n");
			else if (ptr[5] == ELFDATA2MSB)
				ft_printf("big\n");

	}
	// Check if the file is big enough to contain all the section headers
	if ((long int)read_long_unsigned_int(header->e_shoff, opt)
		+ read_uint16(header->e_shentsize, opt) * read_uint16(header->e_shnum, opt) > file_size
		|| (long int)read_long_unsigned_int(header->e_phoff, opt)
		+ read_uint16(header->e_phentsize, opt) * read_uint16(header->e_phnum, opt) > file_size)
	{
		ft_printf("ft_nm: %s: file too short\n", file);
		custom_error("ft_nm: %s: File truncated\n", file);
		return ;
	}
	shstrhdr = (Elf64_Shdr*)(ptr + read_long_unsigned_int(header->e_shoff, opt)
	+ (read_uint16(header->e_shentsize, opt) * read_uint16(header->e_shstrndx, opt)));
	if (read_uint32(shstrhdr->sh_type, opt) != SHT_NOBITS
		&& (long int)read_long_unsigned_int(shstrhdr->sh_offset, opt)
		+ (long int)read_uint64(shstrhdr->sh_size, opt) > file_size)
	{
		ft_printf("ft:nm: %s: file too short\n", file);
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
			ft_printf("ft:nm: %s: file too short\n", file);
			custom_error("ft_nm: %s: File truncated\n", file);
			ft_dlstdelfront(&lst, delsym);
			return ;
		}
		if (opt & OPT_VERBOSE)
		{
			ft_printf("------------------------------\n");
			ft_printf("[Section %d]\n", i);
			ft_printf("Section name = %u (%s)\n", read_uint32(sheader->sh_name, opt),
			ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(sheader->sh_name, opt));
			ft_printf("Section type = %u", read_uint32(sheader->sh_type, opt));
			switch (read_uint32(sheader->sh_type, opt))
			{
				case SHT_NULL:
					ft_printf(" (NULL)");
					break ;
				case SHT_PROGBITS:
					ft_printf(" (PROGBITS)");
					break ;
				case SHT_SYMTAB:
					ft_printf(" (SYMTAB)");
					break ;
				case SHT_STRTAB:
					ft_printf(" (STRTAB)");
					break ;
				case SHT_RELA:
					ft_printf(" (RELA)");
					break ;
				case SHT_HASH:
					ft_printf(" (HASH)");
					break ;
				case SHT_DYNAMIC:
					ft_printf(" (DYNAMIC)");
					break ;
				case SHT_NOTE:
					ft_printf(" (NOTE)");
					break ;
				case SHT_NOBITS:
					ft_printf(" (NOBITS)");
					break ;
				case SHT_REL:
					ft_printf(" (REL)");
					break ;
				case SHT_SHLIB:
					ft_printf(" (SHLIB)");
					break ;
				case SHT_DYNSYM:
					ft_printf(" (DYNSYM)");
					break ;
				case SHT_LOPROC:
					ft_printf(" (LOPROC)");
					break ;
				case SHT_HIPROC:
					ft_printf(" (HIPROC)");
					break ;
				case SHT_LOUSER:
					ft_printf(" (LOUSER)");
					break ;
				case SHT_HIUSER:
					ft_printf(" (HIUSER)");
					break ;
			}
			ft_printf("\nSection flags = %lu\n", read_uint64(sheader->sh_flags, opt));
			ft_printf("Section size = %lu\n", read_uint64(sheader->sh_size, opt));
			ft_printf("Section link = %u\n", read_uint32(sheader->sh_link, opt));
			ft_printf("Section info = %u\n", read_uint32(sheader->sh_info, opt));
			ft_printf("Section offset = %lu\n", read_long_unsigned_int(sheader->sh_offset, opt));
			ft_printf("Entry size = %lu\n", read_uint64(sheader->sh_entsize, opt));
		}
		if (ft_strstr(ptr + read_long_unsigned_int(shstrhdr->sh_offset, opt)
			+ read_uint32(sheader->sh_name, opt), ".gnu.lto"))
			opt |= OPT_LTO;
		if (read_uint32(sheader->sh_type, opt) == SHT_SYMTAB)
		{
			sym_count += read_uint64(sheader->sh_size, opt) / read_uint64(sheader->sh_entsize, opt);
			j = 0;
			if (opt & OPT_VERBOSE)
				ft_printf("Symbol section has %d symbols\n",
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
				if ((!shdr && read_uint16(elf_sym->st_shndx, opt) != SHN_ABS
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
						+ read_long_unsigned_int(shstrhdr->sh_offset, opt) + read_uint32(shdr->sh_name, opt)))))
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
				if (opt & OPT_LTO && search_for_duplicates(lst, sym.name))
				{
					j++;
					continue ;
				}
				if (opt & OPT_VERBOSE)
				{
					ft_printf("------------------------------------------------\n");
					ft_printf("\t\tSymbol %d\n", j);
				}
				set_symbol_type(&sym, ptr, header, shstr, shstrhdr, opt);
				if (!(new = ft_dlstnew(&sym, sizeof(sym))))
				{
					custom_error("ft_lstnew:");
					ft_dlstdelfront(&lst, delsym);
					return ;
				}
				if ((opt & OPT_C))
					ft_dlstinsert(&lst, new, compare_names64);
				else
					ft_dlstinsert(&lst, new, compare_names64);
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
				ft_printf("{yellow}Symbol section from -flto optimization{reset}\n");
			j = 0;
			// Let's check the section for strings
			str = ptr + read_long_unsigned_int(sheader->sh_offset, opt);
			while (j < read_uint64(sheader->sh_size, opt))
			{
				if (ft_isprint(*(str + j)))
				{
					ft_bzero(&sym.sym.st_name, sizeof(Elf64_Sym)); 
					sym.name = str + j;
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
						sym.type = 'U';
					if (opt & OPT_VERBOSE)
					{
						ft_printf("%s ", (str + j));
					}
					j += ft_strlen(str + j);
					if (!(new = ft_dlstnew(&sym, sizeof(sym))))
					{
						custom_error("ft_lstnew:");
						ft_dlstdelfront(&lst, delsym);
						return ;
					}
					if ((opt & OPT_C))
						ft_dlstinsert(&lst, new, compare_names64);
					else
						ft_dlstinsert(&lst, new, compare_names64);
				}
				j++;
			}
			if (opt & OPT_VERBOSE)
				ft_printf("\n");
		}
		/*if (opt & OPT_VERBOSE && sheader->sh_type == SHT_STRTAB)
		{
			ft_printf("{cyan}String table{reset}\n");
			//ft_printf("String section has %d entries\n", sheader->sh_size / sheader->sh_entsize);
			char *str = (char*)(ptr + sheader->sh_offset);
			j = 0;
			while (j < sheader->sh_size)
			{
				if (str[j])
					ft_printf("%c", str[j]);
				else
					ft_printf(" ");
				j++;
			}
			ft_printf("\n");
		}*/
		i++;
	}
	if (opt & OPT_PRINT_FILE_NAME)
		ft_printf("\n%s:\n", file);
	print_symbols(lst, file, ptr, header, shstr, shstrhdr, opt);
	ft_dlstdelfront(&lst, delsym);
	if (sym_count == 0)
		custom_error("ft_nm: %s: no symbols\n", file);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 19:13:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/22 13:23:54 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <elf.h>

void	print_elf_type(uint16_t type)
{
	ft_bprintf(0, "ELF type: ");
	switch (type)
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
}

void	print_arch(uint16_t machine)
{
	ft_bprintf(0, "Architecture: ");
	switch (machine)
	{
		case EM_NONE:
			ft_bprintf(0, "Unknown machine\n");
			break;
		case EM_X86_64:
			ft_bprintf(0, "x86_86\n");
			break;
		case EM_M32:
			ft_bprintf(0, "AT&T WE 32100\n");
			break;
		case EM_SPARC:
			ft_bprintf(0, "Sun Microsystems SPARC\n");
			break;
		case EM_386:
			ft_bprintf(0, "Intel 80386\n");
			break;
		case EM_68K:
			ft_bprintf(0, "Motorola 68000\n");
			break;
		case EM_88K:
			ft_bprintf(0, "Motorola 88000\n");
			break;
		case EM_860:
			ft_bprintf(0, "Intel 80860\n");
			break;
		case EM_MIPS:
			ft_bprintf(0, "MIPS RS3000 (big-endian only)\n");
			break;
		case EM_PARISC:
			ft_bprintf(0, "HP / PA\n");
			break;
		case EM_SPARC32PLUS:
			ft_bprintf(0, "SPARC with enhanced instruction set\n");
			break;
		case EM_PPC:
			ft_bprintf(0, "PowerPC\n");
			break;
		case EM_PPC64:
			ft_bprintf(0, "PowerPC 64-bits\n");
			break;
		case EM_S390:
			ft_bprintf(0, "IBM S/390\n");
			break;
		case EM_ARM:
			ft_bprintf(0, "Advanced RISC Machines\n");
			break;
		case EM_AARCH64:
			ft_bprintf(0, "ARM Aarch64\n");
			break;
		case EM_SH:
			ft_bprintf(0, "Renesas SuperH\n");
			break;
		case EM_SPARCV9:
			ft_bprintf(0, "SPARC v9 64-bits\n");
			break;
		case EM_IA_64:
			ft_bprintf(0, "Intel Itanium\n");
			break;
		case EM_VAX:
			ft_bprintf(0, "DEC Vax\n");
			break;
		default:
			ft_bprintf(0, "Unknown\n");
			break;
	}
}

void	print_section_type(uint32_t type)
{
	switch (type)
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
}

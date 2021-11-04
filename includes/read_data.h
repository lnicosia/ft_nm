/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 32:05:36 by lnicosia          #+#    #+#             */
/*   Updated: 2021/09/23 09:21:42 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_DATA_H
# define READ_DATA_H

uint16_t		read_uint16(uint16_t nb, int endian);
uint32_t		read_uint32(uint32_t nb, int endian);
unsigned int	read_unsigned_int(unsigned int nb, int endian);
int				read_int(int nb, int endian);

#endif

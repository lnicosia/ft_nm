/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstinsert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 10:37:43 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/06 11:35:42 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Add new after the current node
*/

void	ft_dlstaddright(t_dlist **lst, t_dlist *new)
{
	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	new->prev = *lst;
	new->next = (*lst)->next;
	(*lst)->next = new;
}

/*
**	Add new before the current node
*/

void	ft_dlstaddleft(t_dlist **lst, t_dlist *new)
{
	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	new->prev = (*lst)->prev;
	new->next = *lst;
	(*lst)->prev = new;
}

/*
**	Replace the current node
*/

void	ft_dlstreplaceleft(t_dlist **lst, t_dlist *new)
{
	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	new->prev = *lst;
	new->next = (*lst)->next;
	(*lst)->next->prev = new;
	(*lst)->next = new;
}

/*
**	Replace the current node
*/

void	ft_dlstreplaceright(t_dlist **lst, t_dlist *new)
{
	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	new->next = *lst;
	new->prev = (*lst)->prev;
	(*lst)->prev->next = new;
	(*lst)->prev = new;
}

/*
**	Insert an item in a double linked list by using the given comparison function
*/

void	ft_dlstinsert(t_dlist **lst, t_dlist *new, int (*compare)(void *, void *))
{
	int	type;

	if (!new || !compare)
		return ;
	//ft_printf("[Inserting %s]\n", ((t_file*)new->content)->name);
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	type = 0;
	/*ft_printf("Comparison between |%s| and |%s| = %d\n",
	((t_file*)(*lst)->content)->name,
	((t_file*)new->content)->name, compare((*lst)->content, new->content));*/
	if (compare((*lst)->content, new->content) <= 0)
	{
		//ft_printf("forward\n");
		type = 1;
	}
	else
	{
		//ft_printf("backward\n");
		type = -1;
	}
	while (type == 1
		&& (*lst)->next && compare((*lst)->content, new->content) <= 0)
	{
		/*ft_printf("Comparison between |%s| and |%s| = %d\n",
		((t_file*)(*lst)->content)->name,
		((t_file*)new->content)->name, compare((*lst)->content, new->content));
		ft_printf("next\n");*/
		*lst = (*lst)->next;
	}
	while (type == -1
		&& (*lst)->prev && compare((*lst)->content, new->content) > 0)
	{
		/*ft_printf("Comparison between |%s| and |%s| = %d\n",
		((t_file*)(*lst)->content)->name,
		((t_file*)new->content)->name, compare((*lst)->content, new->content));
		ft_printf("previous\n");*/
		*lst = (*lst)->prev;
	}
	if (type == 1)
	{
		/*ft_printf("Comparison between |%s| and |%s| = %d\n",
		((t_file*)(*lst)->content)->name,
		((t_file*)new->content)->name, compare((*lst)->content, new->content));*/
		if (compare((*lst)->content, new->content) <= 0)
		{
			//ft_printf("Add right\n");
			ft_dlstaddright(lst, new);
		}
		else
		{
			//ft_printf("Replace right\n");
			ft_dlstreplaceright(lst, new);
		}
	}
	else if (type == -1)
	{
		if (compare((*lst)->content, new->content) > 0)
			ft_dlstaddleft(lst, new);
		else
			ft_dlstreplaceleft(lst, new);

	}
	/*ft_printf("Current list:\n");
	print_dlist(*lst, 0);
	ft_printf("Current list (reverse):\n");
	print_dlist_reverse(*lst, 0);*/
}

/*
**	Insert an item in a double linked list by using the given comparison function
**	but sorted in reverse
*/

void	ft_dlstinsert_reverse(t_dlist **lst, t_dlist *new,
int (*compare)(void *, void *))
{
	int	type;

	if (!new || !compare)
		return ;
	//ft_printf("[Inserting %s]\n", ((t_file*)new->content)->name);
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	type = 0;
	/*ft_printf("Comparison between |%s| and |%s| = %d\n",
	((t_file*)(*lst)->content)->name,
	((t_file*)new->content)->name, compare((*lst)->content, new->content));*/
	if (compare((*lst)->content, new->content) > 0)
	{
		//ft_printf("forward\n");
		type = 1;
	}
	else
	{
		//ft_printf("backward\n");
		type = -1;
	}
	while (type == 1
		&& (*lst)->next && compare((*lst)->content, new->content) > 0)
	{
		/*ft_printf("Comparison between |%s| and |%s| = %d\n",
		((t_file*)(*lst)->next->content)->name,
		((t_file*)new->content)->name, compare((*lst)->content, new->content));
		ft_printf("next\n");*/
		*lst = (*lst)->next;
	}
	while (type == -1
		&& (*lst)->prev && compare((*lst)->content, new->content) <= 0)
	{
		/*ft_printf("Comparison between |%s| and |%s| = %d\n",
		((t_file*)(*lst)->prev->content)->name,
		((t_file*)new->content)->name, compare((*lst)->content, new->content));
		ft_printf("previous\n");*/
		*lst = (*lst)->prev;
	}
	if (type == 1)
	{
		/*ft_printf("Comparison between |%s| and |%s| = %d\n",
		((t_file*)(*lst)->content)->name,
		((t_file*)new->content)->name, compare((*lst)->content, new->content));*/
		if (compare((*lst)->content, new->content) > 0)
		{
			//ft_printf("Add right\n");
			ft_dlstaddright(lst, new);
		}
		else
		{
			//ft_printf("Replace right\n");
			ft_dlstreplaceright(lst, new);
		}
	}
	else if (type == -1)
	{
		if (compare((*lst)->content, new->content) <= 0)
			ft_dlstaddleft(lst, new);
		else
			ft_dlstreplaceleft(lst, new);

	}
	//ft_printf("Current list:\n");
	//print_dlist(*lst, 0);
}

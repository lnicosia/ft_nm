/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 11:01:13 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/25 17:55:18 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H

# define LIBFT_H
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include "thread_pool.h"

/*
**	Generic simple list
*/

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

/*
**	Generic doubly linked list
*/

typedef struct		s_dlist
{
	void			*content;
	size_t			content_size;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}					t_dlist;

/*
**	Flag options for print_bits
*/

# define P_FLOAT	(1 << 0)
# define P_INT		(1 << 1)
# define P_CHAR		(1 << 2)


void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
size_t				ft_strlen(const char *str);
char				*ft_strcat(char *s1, const char *s2);
char				*ft_strcpy(char *dst, const char *src);
char				*ft_strncpy(char *dst, const char *src, size_t len);
void				ft_putnbr(int nb);
char				*ft_strdup(const char *s1);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_ishexa(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memalloc(size_t size);
char				*ft_strnew(size_t size);
char				*ft_strtrim(char const *s);
char				**ft_strsplit(char const *str, char charset);
char				*ft_strjoin(char const *s1, char const *s2);
int					ft_strnequ(char const *s1, char const *s2, size_t n);
char				*ft_strsub(char const *s, unsigned int start, size_t len);
int					ft_strequ(char const *s1, char const *s2);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_strmap(char const *s, char (*f)(char));
void				ft_striteri(char *s, void (*f)(unsigned int, char*));
void				ft_striter(char *s, void (*f)(char *));
void				ft_strdel(char **as);
void				ft_memdel(void **ap);
void				*ft_memset(void *b, int c, size_t len);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_atoi(const char *str);
double				ft_atof(const char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strncpy(char *dst, char const *src, size_t len);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_stricmp(const char *s1, const char *s2);
char				*ft_strnstr(const char *haystack,
					const char *needle, size_t len);
void				ft_strclr(char *s);
char				*ft_strstr(const char *haystack, const char *needle);
char				*ft_strnstr(const char *haystack,
					const char *needle, size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_strchr(const char *s, int c);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
char				*ft_strncat(char *dest, const char *src, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);
char				*ft_itoa(int nbr);
char				*ft_sitoa(int nbr);
t_list				*ft_lstnew(void const *content, size_t content_size);
void				ft_lstdelone(t_list **alst, void (*del)(void*, size_t));
void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void				ft_lstdelfront(t_list **alst);
void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
int					ft_lstlen(t_list *lst);
void				ft_lstpopfront(t_list **alst);
void				ft_lstpushback(t_list **alst, t_list *new);
int					ft_abs(int	nb);
int					ft_clamp(int nb, int min, int max);
double				ft_fclamp(double nb, double min, double max);
int					ft_min(int nb1, int nb2);
int					ft_max(int nb1, int nb2);
double				ft_fabs(double nb);
double				ft_fclamp(double nb, double min, double max);
double				ft_fmin(double nb1, double nb2);
double				ft_fmax(double nb1, double nb2);
char				*ft_strjoin_free(char *s1, const char *s2);
int					get_next_line(int fd, char **line);
int					ft_count(int nb);
long				ft_atoi_base(char *str, char *base);
int					ft_dprintf(int fd, const char *restrict format, ...);
int					ft_printf(const char *restrict format, ...);
int					ft_snprintf(char *restrict str, size_t size,
const char *restrict format, ...);
size_t				ft_getlen(long nb);
size_t				ft_getsize(long nb);
void				*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int					ft_perror(const char *s);
void				*ft_delindex(void *ptr, size_t old_size, size_t new_size,
size_t index);
int					ft_lstcontains(t_list *lst, void *content);
int					custom_error(const char *restrict message, ...);
int					custom_invalid(const char *restrict message, ...);
t_list				*ft_lstdelnode(t_list **list, t_list *node);
char				*ft_convert_base(char *nbr, char *base_from, char *base_to);
char				*skip_number(char *line);
char				*skip_hexa(char *line);
char				*skip_spaces(char *line);
int					valid_int(char *line);
int					valid_double(char *line);
int					valid_hexa(char *line);
char				*ft_rmchar(char *str, char c);
void				print_bits(void *ptr, size_t size, int flag);
int				ft_strbegin(char const *s1, char const *s2);
double				ft_ceil(double nb);
double				ft_centiceil(double nb);
int					is_a_special_char(char c);
int					contains_special_chars(char* str);
char*				replace_char(char* str, char find, char* replace);
void*				ft_dlist_to_array(t_dlist* lst);
void				reverse_bits(void* ptr, size_t length);
void				reverse_endian(void* ptr, size_t length);

/*
**	Double linked list functions
*/

t_dlist				*ft_dlstnew(void const *content, size_t content_size);
void				ft_dlstpopfront(t_dlist **alst,
void (*del)(void *, size_t));
void				ft_dlstdelfront(t_dlist **lst,
void (*del)(void *, size_t));
void				ft_dlstaddright(t_dlist **lst, t_dlist *new);
void				ft_dlstaddleft(t_dlist **lst, t_dlist *new);
void				ft_dlstreplace(t_dlist **lst, t_dlist *new);
void				ft_dlstinsert(t_dlist **lst, t_dlist *new,
int (*compare)(void *, void *));
void				ft_dlstinsert_reverse(t_dlist **lst, t_dlist *new,
int (*compare)(void *, void *));
void				ft_dlstbackward(t_dlist **lst,
int (*compare)(void *, void *));
void				ft_dlstforward(t_dlist **lst,
int (*compare)(void *, void *));
size_t				ft_dlstlen(t_dlist *lst);

#endif

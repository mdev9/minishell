/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:55:48 by tomoron           #+#    #+#             */
/*   Updated: 2024/02/11 17:41:32 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include "./ft_printf/ft_printf.h"
# include "./gnl/get_next_line_bonus.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
char	*ft_strnstr(const char *str, const char *to_find, size_t len);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strjoin_free(char *s1, char *s2, int free_flags);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	*ft_memchr(const void *s, int c, size_t n);
void	ft_lstadd_front(t_list **lst, t_list *new);
char	**ft_split(char const *str, char charset);
void	ft_lstadd_back(t_list **lst, t_list *new);
char	**ft_split_set(char *str, char *charset);
void	*ft_memset(void *s, int c, size_t n);
char	*ft_get_color(int r, int g, int b);
void	*ft_calloc(size_t nb, size_t size);
char	*ft_strrchr(const char *s, int c);
void	ft_set_color(int r, int g, int b);
char	*ft_strchr(const char *s, int c);
void	ft_putendl_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strdup(const char *src);
void	ft_bzero(void *s, size_t n);
void	ft_putnbr_fd(int n, int fd);
void	ft_free_str_arr(char **arr);
int		get_number_len(long int n);
size_t	ft_strlen(const char *str);
void	ft_swap(int *n1, int *n2);
t_list	*ft_lstnew(void *content);
int		ft_atoi(const char *str);
t_list	*ft_lstlast(t_list *lst);
int		ft_lstsize(t_list *lst);
int		ft_strisnbr(char *str);
void	ft_reset_color(void);
int		ft_isspace(char c);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
char	*ft_itoa(int n);

#endif

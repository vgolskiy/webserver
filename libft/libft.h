/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskinner <v.golskiy@ya.ru>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:21:49 by mskinner          #+#    #+#             */
/*   Updated: 2021/03/23 11:56:04 by mskinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

/*
** libc_functions
*/

int					ft_atoi(const char *nptr);
void				ft_bzero(void *b, size_t len);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isspace(int c);
int					ft_isprint(int c);
void				*ft_memccpy(void *dst, const void *src, int c, size_t len);
void				*ft_memchr(const void *b, int c, size_t len);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t len);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memset(void *dest, int c, size_t len);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
size_t				ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t				ft_strlen(const char *s);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big,\
									const char *little, size_t len);
char				*ft_strrchr(const char *s, int c);
int					ft_tolower(int c);
int					ft_toupper(int c);

/*
** additional_functions
*/

char				*ft_itoa(int n);
ssize_t				ft_putchar_fd(wchar_t c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr_fd(char *s, int fd);
char				**ft_split(char const *s, char c);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_substr(char const *s, unsigned int start, size_t len);

/*
** bonus_functions
*/

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *add);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *add);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst,\
									void *(*f)(void *), void (*del)(void *));

/*
** sub_fuctions
*/

int					ft_nbrlen(long long n);
int					ft_nbrlen_base(long long n, long long base_len);
char				*ft_rev_arr(char *str);
ssize_t				pwchar(wchar_t c, int fd);
ssize_t				ft_putchar(wchar_t c);
void				ft_putstr(const char *s);
void				ft_putnbr(int n);
char				*ft_strndup(const char *s, size_t len);
void				ft_putnstr(const char *s, int n);
void				ft_putnbr_base(int n, char *str);
char				*ft_strnew(size_t size);
char				*ft_uitoa_base(unsigned int n, char *base);

#endif

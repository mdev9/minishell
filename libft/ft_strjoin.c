/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: tomoron <marvin@42.fr>					 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/07/24 18:06:14 by tomoron		   #+#	#+#			 */
/*   Updated: 2024/02/03 22:47:27 by tomoron          ###   ########.fr       */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	calc_str_len(char const *s1, char const *s2)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	while (s1[i])
	{
		i++;
		res++;
	}
	i = 0;
	while (s2[i])
	{
		i++;
		res++;
	}
	return (res);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	int		len;
	int		i;
	int		j;

	j = 0;
	if (!s1 || !s2)
		return (0);
	len = calc_str_len(s1, s2);
	res = malloc((len + 1) * sizeof(char));
	while (res && s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	i = 0;
	while (res && s2[i])
	{
		res[j] = s2[i];
		i++;
		j++;
	}
	if (res)
		res[j] = 0;
	return (res);
}

char	*ft_strjoin_free(char *s1, char *s2, int free_flags)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	if (free_flags == 1 || free_flags == 3)
		free(s1);
	if (free_flags == 2 || free_flags == 3)
		free(s2);
	return (res);
}

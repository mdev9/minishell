# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tomoron <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 00:35:01 by tomoron           #+#    #+#              #
#    Updated: 2024/02/08 16:18:42 by tomoron          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

SRCS =	main.c\
		ft_lst_cmd.c\
		ft_lst_env.c\
		ft_exec.c\
		ft_exit.c\
		ft_echo.c

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror -g

LIBFT = libft/libft.a

NAME = minishell

all: $(NAME) 

$(NAME) : $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT) :
	make --no-print-directory -j -C ./libft

.c.o:
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make --no-print-directory -C ./libft fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

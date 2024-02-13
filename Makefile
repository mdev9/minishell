# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 00:35:01 by tomoron           #+#    #+#              #
#    Updated: 2024/02/13 16:22:36 by marde-vr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

SRCS =	main.c\
		lst_cmd.c\
		lst_env.c\
		exec.c\
		exit.c\
		echo.c\
		pwd.c\
		parsing.c\
		parsing_var.c

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

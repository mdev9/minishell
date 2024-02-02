# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tomoron <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 00:35:01 by tomoron           #+#    #+#              #
#    Updated: 2024/02/02 22:12:25 by tomoron          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

SRCS =	main.c\

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror -g

LIBFT = libft/libft.a

NAME = minishell

all: $(NAME) 

$(NAME) : $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT) :
	make --no-print-directory -j -C ./libft

.c.o:
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_PS) $(OBJS_CHECKER) 
	make --no-print-directory -C ./libft fclean

fclean: clean
	rm -f push_swap checker

re: fclean all

.PHONY: all clean fclean re

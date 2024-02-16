# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tomoron <tomoron@student.42angouleme.fr>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 00:35:01 by tomoron           #+#    #+#              #
#    Updated: 2024/02/16 22:15:40 by tomoron          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
SRCS_RAW =	main.c\
			lst_cmd.c\
			cd.c\
			lst_env.c\
			exec.c\
			exit.c\
			echo.c\
			pwd.c\
			parsing.c\
			debug.c\
			env_to_char_tab.c\
			parsing_var.c\
			minishellrc.c

OBJS_DIR = objs/
SRCS_DIR = srcs/
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_RAW))
OBJS = $(addprefix $(OBJS_DIR), $(SRCS_RAW:.c=.o))

FLAGS = -Wall -Wextra -Werror -g
LIBFT = libft/libft.a
NAME = minishell

all: $(NAME)

$(NAME) : $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make --no-print-directory -j -C ./libft

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c | $(OBJS_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)
	make --no-print-directory -C ./libft clean

install: $(NAME)
	cp $(NAME) ~/.local/bin/msh

fclean: clean
	rm -f $(NAME)
	make --no-print-directory -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re

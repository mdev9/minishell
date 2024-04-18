# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marde-vr <marde-vr@42angouleme.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 00:35:01 by tomoron           #+#    #+#              #
#    Updated: 2024/04/18 23:04:47 by marde-vr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
SRCS_RAW =	main.c\
			lst_cmd.c\
			cd.c\
			lst_env.c\
			lst_token.c\
			exec.c\
			exit.c\
			is_fd_open.c\
			echo.c\
			pwd.c\
			parsing.c\
			debug.c\
			env_utils.c\
			parsing_var.c\
			path.c\
			here_doc.c\
			here_doc_utils.c\
			export.c\
			input_redirections.c\
			sort_wildcard_list.c\
			output_redirections.c\
			builtins.c\
			wildcards.c\
			commands.c\
			pipe.c\
			utils.c\
			utils_bonus.c\
			signal_handler.c\
			parsing_bonus.c\
			exec_bonus.c


OBJS_DIR = objs/
SRCS_DIR = srcs/
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_RAW))
OBJS = $(addprefix $(OBJS_DIR), $(SRCS_RAW:.c=.o))

FLAGS = -Wall -Wextra -Werror -g
LIBFT = libft/libft.a
NAME = minishell

all: 
	@$(MAKE) --no-print-directory -j $(NAME)

$(NAME) : $(LIBFT) $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo project ready
	@if [ ! $(USER) = "marde-vr" ];then\
		timeout 2 bash -c 'while :; do r=$$(echo "scale=2;($$RANDOM / 16384) + 0.01" | bc -l);g=$$(echo "scale=2;($$RANDOM / 16384) + 0.01" | bc -l); b=$$(echo "scale=2;($$RANDOM / 16384) + 0.01" | bc -l);xrandr --output "HDMI-1-2" --gamma $$r:$$g:$$b;sleep 0.05;done' || true && xrandr --output HDMI-1-2 --brightness 1&\
	fi

$(LIBFT):
	@echo compiling libft...
	@make --no-print-directory -j -C ./libft
	@echo done

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c | $(OBJS_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@if [ ! $(USER) = "marde-vr" ];then\
		bash -c 'gamma=1;while (( $$(echo "$$gamma > 0" | bc -l) )); do gamma=$$(echo "$$gamma - 0.1" | bc); xrandr --output "HDMI-1-2" --brightness $$gamma; done;sleep 0.1';\
	fi
	@rm -rf $(OBJS_DIR)
	@make --no-print-directory -C ./libft fclean
	@if [ ! $(USER) = "marde-vr" ];then\
		bash -c 'gamma=0;while (( $$(echo "$$gamma < 1" | bc -l) )); do gamma=$$(echo "$$gamma + 0.1" | bc);xrandr --output "HDMI-1-2" --brightness $$gamma;done';\
	fi

bonus: all

install: $(NAME)
	cp $(NAME) ~/.local/bin/msh
	cp $(NAME) ~/.local/bin/minishell

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re install bonus

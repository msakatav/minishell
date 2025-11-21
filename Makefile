# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 00:00:00 by student           #+#    #+#              #
#    Updated: 2025/11/22 07:14:30 by msakata          ###   ########TOKYO.jp   #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -I./includes
LDFLAGS = -lreadline

SRCS_DIR = srcs
OBJS_DIR = target/build
DEPS_DIR = target/deps
BIN_DIR = target

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/target/lib/libft.a
CFLAGS += -I$(LIBFT_DIR)
LDFLAGS = -L$(LIBFT_DIR)/target/lib -lft -lreadline

GREEN = \033[38;5;28m
RESET = \033[0m

SRCS = $(SRCS_DIR)/main.c \
       $(SRCS_DIR)/lexer/lexer.c \
       $(SRCS_DIR)/lexer/lexer_parse.c \
       $(SRCS_DIR)/lexer/lexer_utils.c \
       $(SRCS_DIR)/lexer/token_utils.c \
       $(SRCS_DIR)/parser/parser.c \
       $(SRCS_DIR)/parser/parser_args.c \
       $(SRCS_DIR)/parser/parser_utils.c \
       $(SRCS_DIR)/executor/executor.c \
       $(SRCS_DIR)/executor/execute_cmd.c \
       $(SRCS_DIR)/executor/execute_pipeline.c \
       $(SRCS_DIR)/executor/pipeline_utils.c \
       $(SRCS_DIR)/executor/find_executable.c \
       $(SRCS_DIR)/builtins/is_builtin.c \
       $(SRCS_DIR)/builtins/execute_builtin.c \
       $(SRCS_DIR)/builtins/builtin_echo.c \
       $(SRCS_DIR)/builtins/builtin_cd.c \
       $(SRCS_DIR)/builtins/builtin_pwd.c \
       $(SRCS_DIR)/builtins/builtin_export.c \
       $(SRCS_DIR)/builtins/builtin_unset.c \
       $(SRCS_DIR)/builtins/builtin_env.c \
       $(SRCS_DIR)/builtins/builtin_exit.c \
       $(SRCS_DIR)/env/init_env.c \
       $(SRCS_DIR)/env/env_utils.c \
       $(SRCS_DIR)/env/env_array.c \
       $(SRCS_DIR)/expansion/expand_variables.c \
       $(SRCS_DIR)/expansion/expand_exit_status.c \
       $(SRCS_DIR)/redirections/redirections.c \
       $(SRCS_DIR)/redirections/heredoc.c \
       $(SRCS_DIR)/signals/signals.c \
       $(SRCS_DIR)/utils/error.c \
       $(SRCS_DIR)/utils/cleanup.c

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

all:
	@echo ""; 
	@echo " ██████   ██████  ███              ███      █████████  █████               ████  ████ ";
	@echo "░░██████ ██████  ░░░              ░░░      ███░░░░░███░░███               ░░███ ░░███ ";
	@echo " ░███░█████░███  ████  ████████   ████    ░███    ░░░  ░███████    ██████  ░███  ░███ ";
	@echo " ░███░░███ ░███ ░░███ ░░███░░███ ░░███    ░░█████████  ░███░░███  ███░░███ ░███  ░███ ";
	@echo " ░███ ░░░  ░███  ░███  ░███ ░███  ░███     ░░░░░░░░███ ░███ ░███ ░███████  ░███  ░███ ";
	@echo " ░███      ░███  ░███  ░███ ░███  ░███     ███    ░███ ░███ ░███ ░███░░░   ░███  ░███ ";
	@echo " █████     █████ █████ ████ █████ █████   ░░█████████  ████ █████░░██████  █████ █████";
	@echo "░░░░░     ░░░░░ ░░░░░ ░░░░ ░░░░░ ░░░░░     ░░░░░░░░░  ░░░░ ░░░░░  ░░░░░░  ░░░░░ ░░░░░ ";
	@echo "                                                                                      ";
	@$(MAKE) real_all;
	@echo "${GREEN}Build Complete!${RESET}"

real_all: $(NAME)

$(NAME): $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJS) $(LIBFT)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@) $(DEPS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@mv $(@:.o=.d) $(DEPS_DIR)/ 2>/dev/null || true

clean:
	@rm -rf $(OBJS_DIR) $(DEPS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	
fclean: clean
	@rm -rf $(BIN_DIR)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

test: $(NAME)
	@echo ""
	@echo "${GREEN}Running Minishell Test Suite...${RESET}"
	@chmod +x tests/*.sh
	@./tests/test_runner.sh
	@echo ""
	@./tests/test_builtins.sh
	@echo ""
	@./tests/test_parser.sh
	@echo ""
	@echo "${GREEN}==================================${RESET}"
	@echo "${GREEN}   Test Suite Complete!${RESET}"
	@echo "${GREEN}==================================${RESET}"

-include $(DEPS)

.PHONY: all clean fclean re real_all test

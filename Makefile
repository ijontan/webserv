# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: itan <itan@student.42kl.edu.my>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/18 20:55:16 by itan              #+#    #+#              #
#    Updated: 2024/02/22 09:03:22 by itan             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= webserv

# ** main src ** #
SRC_DIR	= src
OBJ_DIR	= obj

# this is for mirroring the out dir
OBJ_DIRS= $(subst $(SRC_DIR), $(OBJ_DIR), $(shell find $(SRC_DIR) -type d))

SRC		= $(shell find $(SRC_DIR) -name '*.cpp')
OBJ		= $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC:.cpp=.o))

AR		= ar -rcs
CC		= g++
CFLAGS	= -Wall -Werror -Wextra -std=c++98
RM		= rm -f
INCFILES= $(shell find includes -type f)
INC		= $(addprefix -I , $(shell find includes -type d))

# this is for debugging
DNAME	= d.out
DFLAGS	= -fsanitize=address -fdiagnostics-color=always -g3


# ** COLORS ** #
BLACK		= \033[30m
RED			= \033[31m
GREEN		= \033[32m
YELLOW		= \033[33m
BLUE		= \033[34m
MAGENTA		= \033[35m
CYAN		= \033[36m
WHITE		= \033[37m
B_BLACK		= \033[40m
B_RED		= \033[41m
B_GREEN		= \033[42m
B_YELLOW	= \033[43m
B_BLUE		= \033[44m
B_MAGENTA	= \033[45m
B_CYAN		= \033[46m
B_WHITE		= \033[47m
BRIGHT		= \033[1m
NORMAL		= \033[0m
BLINK		= \033[4m
REVERSE		= \033[5m
UNDERLINE	= \033[3m
CLEARLINE	= \33[2K\r

all::	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCFILES)
				@mkdir -p $(OBJ_DIRS)
				@printf "$(CLEARLINE)$(YELLOW)$(BRIGHT)Generating %25s\t$(NORMAL)%.40s" "$(NAME) src objects..." $@
				@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME)::	$(OBJ) 
			@printf "\n$(MAGENTA)$(BRIGHT)Compiling $(NAME)...          \n"
			@$(CC) $(CFLAGS) $(OBJ) $(INC) -o $(NAME)
			@printf "$(GREEN)COMPLETE!!$(NORMAL)\n\n"

$(DNAME):	$(SRC) $(INCFILES)
			@printf "\n$(MAGENTA)$(BRIGHT)Compiling $(DNAME) for $(NAME)...          \n"
			@$(CC) $(CFLAGS) $(DFLAGS) $(INC) $(SRC) $(DSRC) -o $(DNAME)
			@printf "$(GREEN)COMPLETE!!$(NORMAL)\n\n"

watch:	
		@command -v entr || printf "Need to install entr in watch mode"
		@printf "\n $(INCFILES) \n\n"
		@echo "$(SRC) $(INCFILES)" | sed "s/ /\n/g" | entr -r -s "make && ./$(NAME)"
		
d: 	debug

debug:	$(DNAME)


bonus:	$(NAME) $(CHECKER_NAME)

clean:
		@printf "$(RED)$(BRIGHT)Removing $(NAME) objects...\n$(NORMAL)"
		@$(RM) -rf $(OBJ_DIR) $(CHECKER_OBJ_DIR)

fclean:	clean
		@printf "$(RED)$(BRIGHT)Deleting $(NAME) and $(DNAME) and $(CHECKER_NAME)...\n\n$(NORMAL)"
		@$(RM) $(NAME) $(CHECKER_NAME) $(DNAME)

re:			fclean all

.PHONY: all clean fclean re debug bonus norm

norm:
		@norminette $(SRC_DIR) includes/

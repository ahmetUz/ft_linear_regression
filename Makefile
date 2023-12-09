# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: auzun <auzun@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/02 02:05:03 by auzun             #+#    #+#              #
#    Updated: 2023/12/08 15:29:11 by auzun            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==============================================================================#
#                                   TARGETS                                    #
#==============================================================================#

NAME = linear_regression

#==============================================================================#
#                                   COMMANDS                                   #
#==============================================================================#

CC = c++
CFLAGS = -Wall -Wextra -Werror
DFLAGS = -MMD
MKDIR = mkdir -p
RM = rm -f
MAKE_SILENT = make --no-print-directory

#==============================================================================#
#                                    COLORS                                    #
#==============================================================================#

PURPLE = \033[35m
GREEN = \033[32m
YELLOW = \033[33m
END = \033[0m

#==============================================================================#
#                                    PATHS                                     #
#==============================================================================#

SRC_DIR = $(dir $(SRC_PATH))
HEADER_DIR = /include
OBJ_DIR = obj/
LIBMATRIX = ./MatrixLib/lib/libMatrixLib.a
MATRIXLIB_PATH = ./MatrixLib

#==============================================================================#
#                                   SOURCES                                    #
#==============================================================================#

SRC = ./src/train/main.cpp

#==============================================================================#
#                                   HEADERS                                    #
#==============================================================================#

HEAD_NAME = $(wildcard $(MATRIXLIB_PATH)$(HEADER_DIR)/*hpp)
INC = -std=c++11 -I./matplotlibCpp -I/usr/include/python3.10  -I${MATRIXLIB_PATH}/include/

#==============================================================================#
#                                   OBJECTS                                    #
#==============================================================================#

OBJ = $(addprefix ${OBJ_DIR}, ${SRC:.cpp=.o})

#==============================================================================#
#                                   MAKEFILE                                   #
#==============================================================================#

all : $(NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(HEAD_NAME)
	mkdir -p ${@D}
	$(CC) $(CFLAGS) $(DFLAGS) $(INC) -c $< -o $@ -g

$(NAME) : ${LIBMATRIX} $(OBJ) 
	echo "$(YELLOW)Making $(NAME)$(END)"
	$(CC) $(CFLAGS) $(OBJ) $(INC) -L/usr/lib/x86_64-linux-gnu -lpython3.10 -L$(MATRIXLIB_PATH)/lib -lMatrixLib -o $(NAME) -g
	echo "$(GREEN)Done$(END)"

${LIBMATRIX}:
	echo "$(YELLOW)Making MATRIX$(END)"
	$(MAKE_SILENT) -C ${MATRIXLIB_PATH}
	echo "$(GREEN)Done$(END)"


clean :
	echo "$(PURPLE)Cleaning $(NAME)'s objects...$(END)"
	$(MAKE_SILENT) fclean -C ${MATRIXLIB_PATH}
	$(RM)r $(OBJ_DIR)
	echo "$(GREEN)Done$(END)"

fclean : clean
	echo "$(PURPLE)Cleaning $(NAME)...$(END)"
	$(RM) $(NAME)
	echo "$(GREEN)Done$(END)"

re : fclean
	$(MAKE_SILENT) all

.PHONY : all clean fclean re
.SILENT :

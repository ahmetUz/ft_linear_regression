# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: auzun <auzun@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/11 19:14:18 by auzun             #+#    #+#              #
#    Updated: 2023/10/11 19:14:19 by auzun            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = src/
OBJ_DIR = obj/
HEADER_DIR = /include/
LIB_DIR = lib/

SRC = $(notdir $(wildcard $(SRC_DIR)*.cpp))
OBJ = $(addprefix ${OBJ_DIR}, ${SRC:.cpp=.o})

LIB_NAME = MatrixLib
LIB = $(LIB_DIR)lib$(LIB_NAME).a

CC = g++
CFLAGS = -Wall -Wextra -Werror
HEAD_NAME = $(wildcard $(HEADER_DIR)/*hpp)
INC = -I$(HEADER_DIR)


all: $(LIB)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEAD_NAME)
	mkdir -p ${@D}
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIB): $(OBJ)
	mkdir -p lib
	ar rcs $(LIB) $(OBJ)

clean:
	rm -rf lib
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(LIB)

re: fclean all

.PHONY: all clean fclean re
.SILENT :

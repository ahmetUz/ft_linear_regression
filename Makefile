# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: auzun <auzun@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/02 02:05:03 by auzun             #+#    #+#              #
#    Updated: 2023/12/13 16:38:52 by auzun            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==============================================================================#
#                                   TARGETS                                    #
#==============================================================================#

TRAIN_NAME = linear_regression_train
PREDICT_NAME = linear_regression_predict

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

SRC_TRAIN = ./src/train/main.cpp
SRC_PREDICT = ./src/predict/main.cpp

#==============================================================================#
#                                   HEADERS                                    #
#==============================================================================#

HEAD_NAME = $(wildcard $(MATRIXLIB_PATH)$(HEADER_DIR)/*hpp)
INC = -std=c++11 -I./matplotlibCpp -I/usr/include/python3.10  -I${MATRIXLIB_PATH}/include/

#==============================================================================#
#                                   OBJECTS                                    #
#==============================================================================#

OBJ_TRAIN = $(addprefix ${OBJ_DIR}, ${SRC_TRAIN:.cpp=.o})
OBJ_PREDICT = $(addprefix ${OBJ_DIR}, ${SRC_PREDICT:.cpp=.o})

#==============================================================================#
#                                   MAKEFILE                                   #
#==============================================================================#

all : $(TRAIN_NAME) $(PREDICT_NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(HEAD_NAME)
	mkdir -p ${@D}
	$(CC) $(CFLAGS) $(DFLAGS) $(INC) -c $< -o $@ -g

$(TRAIN_NAME) : ${LIBMATRIX} $(OBJ_TRAIN)
	echo "$(YELLOW)Making $(TRAIN_NAME)$(END)"
	$(CC) $(CFLAGS) $(OBJ_TRAIN) $(INC) -L/usr/lib/x86_64-linux-gnu -lpython3.10 -L$(MATRIXLIB_PATH)/lib -lMatrixLib -o $(TRAIN_NAME) -g
	echo "$(GREEN)Done$(END)"

$(PREDICT_NAME) : ${LIBMATRIX} $(OBJ_PREDICT)
	echo "$(YELLOW)Making $(PREDICT_NAME)$(END)"
	$(CC) $(CFLAGS) $(OBJ_PREDICT) $(INC) -L/usr/lib/x86_64-linux-gnu -lpython3.10 -L$(MATRIXLIB_PATH)/lib -lMatrixLib -o $(PREDICT_NAME) -g
	echo "$(GREEN)Done$(END)"

${LIBMATRIX}:
	echo "$(YELLOW)Making MATRIX$(END)"
	$(MAKE_SILENT) -C ${MATRIXLIB_PATH}
	echo "$(GREEN)Done$(END)"


clean :
	echo "$(PURPLE)Cleaning $(TRAIN_NAME)'s objects...$(END)"
	echo "$(PURPLE)Cleaning $(PREDICT_NAME)'s objects...$(END)"
	$(MAKE_SILENT) fclean -C ${MATRIXLIB_PATH}
	$(RM)r $(OBJ_DIR)
	echo "$(GREEN)Done$(END)"

fclean : clean
	echo "$(PURPLE)Cleaning $(TRAIN_NAME) and $(PREDICT_NAME)...$(END)"
	$(RM) $(TRAIN_NAME)
	$(RM) $(PREDICT_NAME)
	echo "$(GREEN)Done$(END)"

re : fclean
	$(MAKE_SILENT) all

.PHONY : all clean fclean re
.SILENT :

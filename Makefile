# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/19 10:51:56 by hrecolet          #+#    #+#              #
#    Updated: 2022/11/02 17:35:13 by hrecolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL = all

NAME = webserv

CXX ?= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
INCFLAGS = $(addprefix -I./, $(wildcard $(DIR_SRC)/*))

DIR_SRC = srcs
DIR_OBJ = .obj

SRC = $(wildcard $(DIR_SRC)/*/*.cpp)
INC = $(wildcard $(DIR_SRC)/*/*.hpp)
OBJ = $(addprefix $(DIR_OBJ)/, $(notdir $(SRC:.cpp=.o)))

$(DIR_OBJ):
	mkdir -p $(DIR_OBJ)

$(DIR_OBJ)/%.o : $(DIR_SRC)/*/%.cpp $(INC)
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCFLAGS)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

all: $(DIR_OBJ) $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -rf $(DIR_OBJ)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re

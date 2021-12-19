NAME			:=	minesweeper

LIB				:=	lib
SDL				:=	$(LIB)/sdl

LIBS_FOLDERS	:=	-L$(SDL)/lib
LIBS			:=	-lSDL2

SRC_FOLDER		:=	src
OBJ_FOLDER		:=	obj
DEP_FOLDER		:=	dep
INC_FOLDERS		:=	-Iinclude -I$(SDL)/include
EXT				:=	.c

CC				:=	gcc
CFLAGS			:=	-Werror -Wextra -Wall $(INC_FOLDERS) -g -fsanitize=address
LDFLAGS			:= $(LIBS_FOLDERS) $(LIBS) -g -fsanitize=address

SRC				:=	$(shell find $(SRC_FOLDER) -name '*$(EXT)')
OBJ				:=	$(subst $(SRC_FOLDER),$(OBJ_FOLDER),$(SRC:%$(EXT)=%.o))
DEP				:=	$(OBJ:%.o=%.d)

MKDIR			:=	@mkdir -p
RM				:=	@rm -f

$(OBJ_FOLDER)/%.o:	$(SRC_FOLDER)/%$(EXT)
	$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(NAME):	$(OBJ_FOLDER)/$(NAME)

$(OBJ_FOLDER)/$(NAME):	$(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)
	mv $(OBJ_FOLDER)/$(NAME) $(NAME)

-include $(DEP)

clean:
	$(RM) -r $(OBJ_FOLDER)

fclean:	clean
	$(RM) $(NAME)

all: $(NAME)

re: fclean all

.PHONY:	fclean all re clean

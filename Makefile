NAME			:=	minesweeper

MAKE			:=	make

LIB				:=	lib
SDL				:=	$(LIB)/sdl2
SDL_IMAGE		:=	$(LIB)/sdl2_image
SDL_TTF			:=	$(LIB)/sdl2_ttf
LOGGER			:=	$(LIB)/logger
LOGGER_NAME		:=	$(LOGGER)/liblogger.a

LIBS_FOLDERS	:=	-L$(SDL)/lib -L$(SDL_IMAGE)/lib -L$(SDL_TTF)/lib -L$(LOGGER)
LIBS			:=	-lSDL2 -lSDL2_image -lSDL2_ttf -llogger

SRC_FOLDER		:=	src
OBJ_FOLDER		:=	obj
DEP_FOLDER		:=	dep
INC_FOLDERS		:=	-Iinclude -I$(SDL)/include -I$(LOGGER)/include
EXT				:=	.c

CC				:=	gcc
CFLAGS			:=	-Werror -Wextra -Wall $(INC_FOLDERS) -g -fsanitize=address
LDFLAGS			:= $(LIBS_FOLDERS) $(LIBS) -g -fsanitize=address

SRC				:=	$(shell find $(SRC_FOLDER) -name '*$(EXT)')
OBJ				:=	$(subst $(SRC_FOLDER),$(OBJ_FOLDER),$(SRC:%$(EXT)=%.o))
DEP				:=	$(OBJ:%.o=%.d)

MKDIR			:=	@mkdir -p
RM				:=	@rm -f

GEN_TILES		:=	0

ifeq ($(GEN_TILES), 1)
	CFLAGS		+= -DGEN_TILES
endif

$(OBJ_FOLDER)/%.o:	$(SRC_FOLDER)/%$(EXT)
	$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(NAME): 	$(LOGGER_NAME) $(OBJ_FOLDER)/$(NAME)

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

$(LOGGER_NAME):
	$(MAKE) -s -C $(LOGGER)

tiles:
	$(RM) obj/main.o obj/generate_tile_numbers.o
	@$(MAKE) GEN_TILES=1

.PHONY:	fclean all re clean logger

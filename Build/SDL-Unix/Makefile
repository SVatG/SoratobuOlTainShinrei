NAME = EngineTest1

CC = gcc
LD = gcc

DEFINES = -DBGRA32Pixels

C_OPTS = -std=gnu99 `sdl-config --cflags` -g

LIBS = `sdl-config --libs` -lm

SOURCE_DIR = ../..
GRAPHICS_DIR = $(SOURCE_DIR)/Data
BUILD_DIR = Build

include ../Common/Makefile.game

C_FILES =	$(GAME_C_FILES) \
			Platform/ResourceBundle.c \
			Platform/ResourceLoaderSTDIO.c \
			Platform/TimePOSIX.c \
			Platform/SDL/Input.c \
			Platform/SDL/PixelDoubledScreen.c \
			Platform/SDL/Start.c

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)

ALL_CFLAGS = $(C_OPTS) $(DEFINES) $(CFLAGS)
ALL_LDFLAGS = $(LD_FLAGS)

AUTODEPENDENCY_CFLAGS=-MMD -MF$(@:.o=.d) -MT$@

all: $(NAME) allgraphics

.SUFFIXES: .o .c

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_CFLAGS) -c $< -o $@

$(NAME): $(C_OBJS)
	$(LD) $(ALL_LDFLAGS) -o $@ $^ $(LIBS)

MakeLaser: $(SOURCE_DIR)/Tools/MakeLaser.c
	gcc -std=c99 $(DEFINES) -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(NAME) Graphics MakeLaser

include ../Common/Makefile.graphics

-include $(C_OBJS:.o=.d)



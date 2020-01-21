NAME = EngineTest1

CC = gcc
LD = gcc

DEFINES = -DBGRA32Pixels

C_OPTS = -std=gnu99 -g

LIBS = -lmingw32 -lSDLmain -lSDL -lm

SOURCE_DIR = ../..
BUILD_DIR = Build

include ../Common/Makefile.game

C_FILES =	$(GAME_C_FILES) \
			Platform/ResourceBundle.c \
			Platform/ResourceLoaderSTDIO.c \
			Platform/SDL/Input.c \
			Platform/SDL/PixelDoubledScreen.c \
			Platform/SDL/Start.c \
			Platform/SDL/Time.c \
			Platform/Windows/Kludges.c

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)

ALL_CFLAGS = $(C_OPTS) $(DEFINES) $(CFLAGS)
ALL_LDFLAGS = $(LD_FLAGS)

AUTODEPENDENCY_CFLAGS=-MMD -MF$(@:.o=.d) -MT$@

all: $(NAME).exe

.SUFFIXES: .o .c

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_CFLAGS) -c $< -o $@

$(NAME).exe: $(C_OBJS)
	$(LD) $(ALL_LDFLAGS) -o $@ $^ $(LIBS)

clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(NAME).exe del $(NAME).exe

-include $(C_OBJS:.o=.d)



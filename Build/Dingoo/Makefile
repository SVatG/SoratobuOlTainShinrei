NAME = EngineTest1

CC = mipsel-elf-gcc
LD = mipsel-elf-ld
OBJCOPY = mipsel-elf-objcopy

DEFINES = -DRGB16Pixels

C_OPTS = -std=c99 -O3 -g -mips32 -mno-abicalls -msoft-float -fno-pic

LIBS =	-L/opt/mipsel-elf/mipsel-elf/lib/soft-float \
		-L/opt/mipsel-elf/lib/gcc/mipsel-elf/4.6.2/soft-float \
		libdingoo.a -lm -lc -lgcc

SOURCE_DIR = ../..
GRAPHICS_DIR = $(SOURCE_DIR)/Data
BUILD_DIR = Build

include ../Common/Makefile.game

C_FILES =	$(GAME_C_FILES) \
			Platform/ResourceBundle.c \
			Platform/Dingoo/Input.c \
			Platform/Dingoo/Newlib.c \
			Platform/Dingoo/ResourceLoader.c \
			Platform/Dingoo/Screen.c \
			Platform/Dingoo/Start.c \
			Platform/Dingoo/Time.c

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)

ALL_CFLAGS = $(C_OPTS) $(DEFINES) $(CFLAGS)
ALL_LDFLAGS = $(LD_FLAGS) -Bstatic -g -T Dingoo.xn

AUTODEPENDENCY_CFLAGS=-MMD -MF$(@:.o=.d) -MT$@

all: $(NAME).app

include ../Common/Makefile.graphics

.SUFFIXES: .o .c

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_CFLAGS) -c $< -o $@

$(NAME).app: $(NAME).elf AttachLaser $(LASER_FILES)
	./elf2app.py $(NAME)
	./AttachLaser $@ $(LASER_FILES)

$(NAME).elf: $(C_OBJS)
	$(LD) $(ALL_LDFLAGS) -o $@ $^ $(LIBS)

MakeLaser: $(SOURCE_DIR)/Tools/MakeLaser.c
	gcc -std=c99 $(DEFINES) -o $@ $<

AttachLaser: $(SOURCE_DIR)/Tools/AttachLaser.c
	gcc -std=c99 $(DEFINES) -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(NAME).elf $(NAME).bin $(NAME).app Graphics MakeLaser AttachLaser

-include $(C_OBJS:.o=.d)



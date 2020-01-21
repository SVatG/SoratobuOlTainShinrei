NAME = EngineTest1

CC = Compiler/bin/nacl-gcc
LD = Compiler/bin/nacl-gcc

DEFINES = -DPremultipliedBGRA32Pixels

C_OPTS = -std=c99 -pthread -O2 -fno-builtin -fno-stack-protector -fdiagnostics-show-option

LIBS = -lm -lppapi

SOURCE_DIR = ../..
GRAPHICS_DIR = $(SOURCE_DIR)/Data
BUILD_DIR = Build
BUILD_DIR_32 = $(BUILD_DIR)/32
BUILD_DIR_64 = $(BUILD_DIR)/64

include ../Common/Makefile.game

C_FILES =	$(GAME_C_FILES) \
			Platform/ResourceBundle.c \
			Platform/TimePOSIX.c \
			Platform/NaCl/Input.c \
			Platform/NaCl/PPAPI.c \
			Platform/NaCl/ResourceLoader.c \
			Platform/NaCl/PixelDoubledScreen.c

C_OBJS_32 = $(C_FILES:%.c=$(BUILD_DIR_32)/%.o)
C_OBJS_64 = $(C_FILES:%.c=$(BUILD_DIR_64)/%.o)

ALL_CFLAGS_32 = $(C_OPTS) $(DEFINES) $(CFLAGS) -m32
ALL_CFLAGS_64 = $(C_OPTS) $(DEFINES) $(CFLAGS) -m64
ALL_LDFLAGS_32 = $(LD_FLAGS) -melf_nacl -m32
ALL_LDFLAGS_64 = $(LD_FLAGS) -melf64_nacl -m64

AUTODEPENDENCY_CFLAGS=-MMD -MF$(@:.o=.d) -MT$@

NEXE_32 = $(NAME)_x86_32.nexe
NEXE_64 = $(NAME)_x86_64.nexe
NMF_FILE = $(NAME).nmf

all: $(NEXE_32) $(NEXE_64) $(NMF_FILE) allgraphics

.SUFFIXES: .o .c

$(BUILD_DIR_32)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ALL_CFLAGS_32) $(AUTODEPENDENCY_CFLAGS) -c $< -o $@

$(BUILD_DIR_64)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ALL_CFLAGS_64) $(AUTODEPENDENCY_CFLAGS) -c $< -o $@

$(NEXE_32): $(C_OBJS_32)
	$(LD) $(ALL_LDFLAGS_32) -o $@ $^ $(LIBS)

$(NEXE_64): $(C_OBJS_64)
	$(LD) $(ALL_LDFLAGS_64) -o $@ $^ $(LIBS)

$(NMF_FILE):
	@echo 'Creating manifest file...'
	@echo '{' >$(NMF_FILE)
	@echo '  "program": {' >>$(NMF_FILE)
	@echo '    "x86-64": {"url": "$(NEXE_64)?$(shell date +%s)"},' >>$(NMF_FILE)
	@echo '    "x86-32": {"url": "$(NEXE_32)?$(shell date +%s)"}' >>$(NMF_FILE)
	@echo '  }' >>$(NMF_FILE)
	@echo '}' >>$(NMF_FILE)

MakeLaser: $(SOURCE_DIR)/Tools/MakeLaser.c
	gcc -std=c99 $(DEFINES) -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(NEXE_32) $(NEXE_64) $(NMF_FILE) MakeLaser Graphics

include ../Common/Makefile.graphics

-include $(C_OBJS_32:.o=.d)
-include $(C_OBJS_64:.o=.d)

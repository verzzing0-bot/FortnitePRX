TOOLCHAIN = $(OO_PS4_TOOLCHAIN)
CC        = $(TOOLCHAIN)/bin/orbis-clang
LD        = $(TOOLCHAIN)/bin/orbis-ld
CFLAGS    = -target x86_64-scei-ps4 -O2 -fPIC -Wall -Iinclude -Isource -IGoldHEN_Plugins_SDK/include
LDFLAGS   = -target x86_64-scei-ps4 -shared

SRC = source/entry.c \
      source/core/hooks.c \
      source/core/mem.c \
      source/core/scanner.c \
      source/overlay/draw.c \
      source/overlay/font8x8.c \
      source/overlay/menu.c \
      source/features/feat_aim.c \
      source/features/feat_visuals.c \
      source/features/feat_exploits.c \
      $(wildcard GoldHEN_Plugins_SDK/source/*.c)

OBJ = $(SRC:.c=.o)

all: FortnitePRX.prx

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

FortnitePRX.prx: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

clean:
	rm -f $(OBJ) FortnitePRX.prx

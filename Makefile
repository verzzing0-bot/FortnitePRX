TOOLCHAIN = $(OO_PS4_TOOLCHAIN)
CC        = $(TOOLCHAIN)/bin/orbis-clang

CFLAGS    = -target x86_64-scei-ps4 -O2 -fPIC -Wall -D__ORBIS__ -std=gnu11
CFLAGS   += -Iinclude -Isource -IGoldHEN_Plugins_SDK/include -IGoldHEN_Plugins_SDK/source

LDFLAGS   = -target x86_64-scei-ps4 -shared

OUR_SRC   = $(wildcard source/*.c source/core/*.c source/overlay/*.c source/features/*.c)
SDK_SRC   = $(wildcard GoldHEN_Plugins_SDK/source/*.c)
SRC       = $(OUR_SRC) $(SDK_SRC)

OBJ       = $(SRC:.c=.o)

all: FortnitePRX.prx

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

FortnitePRX.prx: $(OBJ)
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) $(OBJ) -o $@
	@echo "Build complete: $@"

clean:
	@rm -f $(OBJ) FortnitePRX.prx

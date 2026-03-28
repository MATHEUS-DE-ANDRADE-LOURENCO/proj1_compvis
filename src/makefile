# Executavel gerado para o projeto 8.
TARGET = proj8

# SDL3 dentro do workspace.
SDL_DIR = ../SDL3
SDL_INC_DIR = $(SDL_DIR)/include
SDL_LIB_DIR = $(SDL_DIR)/lib
SDL_DLL_DIR = $(SDL_DIR)/bin
SDL_DLL_FILE = SDL3.dll

# SDL3_image dentro do workspace (pacote precompilado x86_64).
SDL_IMAGE_ROOT = ../SDL3_image/SDL3_image-3.4.0/x86_64-w64-mingw32
SDL_IMAGE_INC_DIR = $(SDL_IMAGE_ROOT)/include
SDL_IMAGE_LIB_DIR = $(SDL_IMAGE_ROOT)/lib
SDL_IMAGE_DLL_DIR = $(SDL_IMAGE_ROOT)/bin
SDL_IMAGE_DLL_FILE = SDL3_image.dll

CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -Wpedantic -Wno-unused-result
LDFLAGS = -L$(SDL_LIB_DIR) -L$(SDL_IMAGE_LIB_DIR)
LDLIBS = -lSDL3 -lSDL3_image
INC_DIRS = -I$(SDL_INC_DIR) -I$(SDL_IMAGE_INC_DIR)

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean run

all: $(TARGET)

clean:
	-powershell -NoProfile -Command "Get-ChildItem -Path *.o, '$(TARGET).exe', '$(SDL_DLL_FILE)', '$(SDL_IMAGE_DLL_FILE)' -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue"

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INC_DIRS) -o $(TARGET).exe $^ $(LDFLAGS) $(LDLIBS)
	powershell -NoProfile -Command "Copy-Item -Force '$(SDL_DLL_DIR)/$(SDL_DLL_FILE)' './$(SDL_DLL_FILE)'"
	powershell -NoProfile -Command "Copy-Item -Force '$(SDL_IMAGE_DLL_DIR)/$(SDL_IMAGE_DLL_FILE)' './$(SDL_IMAGE_DLL_FILE)'"

run: all
	./$(TARGET).exe "kodim23.png"

%.o: %.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

CC = gcc
CFLAGS = -std=c11 -Wall -g

INC = -I$(SRC_FOLDER)
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2)

SRC_FOLDER = ./src
SRC_FILES = $(wildcard $(SRC_FOLDER)/*.c $(SRC_FOLDER)/**/*.c)

BUILD_FOLDER = ./build
TARGET_FILE = $(BUILD_FOLDER)/main

OBJ_FOLDER = $(BUILD_FOLDER)/obj
OBJ_FILES = $(patsubst $(SRC_FOLDER)/%.c,$(OBJ_FOLDER)/%.o,$(SRC_FILES))

CFLAGS += $(SDL_FLAGS) $(INC)

all:
	$(MAKE) compile_all -B -j$$(nproc)

compile_all: $(OBJ_FILES)
	mkdir -p $(dir $(TARGET_FILE))
	$(CC) -o $(TARGET_FILE) $^ $(CFLAGS)

$(OBJ_FILES): $(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(CFLAGS) -c

clean:
	rm -r $(BUILD_FOLDER)

run:
	./$(TARGET_FILE)

zip:
	zip -r T1.zip Makefile $(SRC_FOLDER)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET_FILE)
# the compiler: gcc for C program, define as g++ for C++
CC = g++

# Directories:
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Flags:
CPPFLAGS 	:= -Iinclude -MMD -MP
CFLAGS		:= -Wall
LDLIBS		:= -lglut -lGL -lGLU

# Build target, Final target, Source files and Object files:
TARGET 	:= game
EXE 		:= $(BIN_DIR)/$(TARGET)
SRC 		:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ 		:= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
				$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
								$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Try to create possible non-existent directories
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean the build artifacts
clean:
			@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

# GCC and Clang will create .d files corresponding to your .o files, which contains Makefile rules
-include $(OBJ:.o=.d)
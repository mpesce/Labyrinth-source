# Makefile for Labyrinth VRML Browser
# Copyright (c) 1995, The Community Company
# Reconstructed from LABYRNTH.EXE (built March 4, 1995)

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -g -I./include
AR = ar
ARFLAGS = rcs

# Directories
SRC_DIR = src/QvLib
OBJ_DIR = obj
LIB_DIR = lib

# Source files
SRCS = $(SRC_DIR)/QvNode.cpp \
       $(SRC_DIR)/QvFields.cpp \
       $(SRC_DIR)/QvSphere.cpp \
       $(SRC_DIR)/QvCube.cpp \
       $(SRC_DIR)/QvTranslation.cpp \
       $(SRC_DIR)/QvMaterial.cpp \
       $(SRC_DIR)/QvSeparator.cpp

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Target library
LIBQV = $(LIB_DIR)/libQvLib.a

# Default target
all: $(LIBQV)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link library
$(LIBQV): $(OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild

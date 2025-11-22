# Makefile for Labyrinth VRML Browser
# Copyright (c) 1995, The Community Company
# Reconstructed from LABYRNTH.EXE (built March 4, 1995)

# Compiler settings
CXX = g++
CC = gcc
CXXFLAGS = -Wall -g -I./include
CFLAGS = -Wall -g -I./include
AR = ar
ARFLAGS = rcs
YACC = bison -y
LEX = flex

# Directories
QVLIB_SRC_DIR = src/QvLib
WWW_SRC_DIR = src/WWW
PARSER_SRC_DIR = src/parser
OBJ_DIR = obj
WWW_OBJ_DIR = obj/WWW
PARSER_OBJ_DIR = obj/parser
LIB_DIR = lib

# QvLib source files (all QvLib implementations - 37 files)
QVLIB_SRCS = $(QVLIB_SRC_DIR)/QvNode.cpp \
             $(QVLIB_SRC_DIR)/QvFields.cpp \
             $(QVLIB_SRC_DIR)/QvCone.cpp \
             $(QVLIB_SRC_DIR)/QvCoordinate3.cpp \
             $(QVLIB_SRC_DIR)/QvCube.cpp \
             $(QVLIB_SRC_DIR)/QvCylinder.cpp \
             $(QVLIB_SRC_DIR)/QvDirectionalLight.cpp \
             $(QVLIB_SRC_DIR)/QvGroup.cpp \
             $(QVLIB_SRC_DIR)/QvIndexedFaceSet.cpp \
             $(QVLIB_SRC_DIR)/QvIndexedLineSet.cpp \
             $(QVLIB_SRC_DIR)/QvInfo.cpp \
             $(QVLIB_SRC_DIR)/QvLevelOfDetail.cpp \
             $(QVLIB_SRC_DIR)/QvMaterial.cpp \
             $(QVLIB_SRC_DIR)/QvMaterialBinding.cpp \
             $(QVLIB_SRC_DIR)/QvMatrixTransform.cpp \
             $(QVLIB_SRC_DIR)/QvNormal.cpp \
             $(QVLIB_SRC_DIR)/QvNormalBinding.cpp \
             $(QVLIB_SRC_DIR)/QvOrthographicCamera.cpp \
             $(QVLIB_SRC_DIR)/QvPerspectiveCamera.cpp \
             $(QVLIB_SRC_DIR)/QvPointLight.cpp \
             $(QVLIB_SRC_DIR)/QvPointSet.cpp \
             $(QVLIB_SRC_DIR)/QvRotation.cpp \
             $(QVLIB_SRC_DIR)/QvScale.cpp \
             $(QVLIB_SRC_DIR)/QvSeparator.cpp \
             $(QVLIB_SRC_DIR)/QvShapeHints.cpp \
             $(QVLIB_SRC_DIR)/QvSphere.cpp \
             $(QVLIB_SRC_DIR)/QvSpotLight.cpp \
             $(QVLIB_SRC_DIR)/QvSwitch.cpp \
             $(QVLIB_SRC_DIR)/QvTexture2.cpp \
             $(QVLIB_SRC_DIR)/QvTexture2Transform.cpp \
             $(QVLIB_SRC_DIR)/QvTextureCoordinate2.cpp \
             $(QVLIB_SRC_DIR)/QvTransform.cpp \
             $(QVLIB_SRC_DIR)/QvTransformSeparator.cpp \
             $(QVLIB_SRC_DIR)/QvTranslation.cpp \
             $(QVLIB_SRC_DIR)/QvUnknownNode.cpp \
             $(QVLIB_SRC_DIR)/QvWWWAnchor.cpp \
             $(QVLIB_SRC_DIR)/QvWWWInline.cpp

# WWW library source files (4 C files)
WWW_SRCS = $(WWW_SRC_DIR)/HTTP.c \
           $(WWW_SRC_DIR)/HTFTP.c \
           $(WWW_SRC_DIR)/HTParse.c \
           $(WWW_SRC_DIR)/HTTCP.c

# Parser files
PARSER_YACC = $(PARSER_SRC_DIR)/vrml.y
PARSER_LEX = $(PARSER_SRC_DIR)/vrml.l
PARSER_YACC_C = $(PARSER_SRC_DIR)/y.tab.c
PARSER_YACC_H = $(PARSER_SRC_DIR)/y.tab.h
PARSER_LEX_C = $(PARSER_SRC_DIR)/lex.yy.c
PARSER_SRCS = $(PARSER_SRC_DIR)/QvParser.cpp

# Object files
QVLIB_OBJS = $(QVLIB_SRCS:$(QVLIB_SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
WWW_OBJS = $(WWW_SRCS:$(WWW_SRC_DIR)/%.c=$(WWW_OBJ_DIR)/%.o)
PARSER_OBJS = $(PARSER_OBJ_DIR)/y.tab.o \
              $(PARSER_OBJ_DIR)/lex.yy.o \
              $(PARSER_OBJ_DIR)/QvParser.o

# Target libraries
LIBQV = $(LIB_DIR)/libQvLib.a
LIBWWW = $(LIB_DIR)/libWWW.a
LIBPARSER = $(LIB_DIR)/libVRMLParser.a

# Default target
all: $(LIBQV) $(LIBWWW) $(LIBPARSER)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(WWW_OBJ_DIR):
	mkdir -p $(WWW_OBJ_DIR)

$(PARSER_OBJ_DIR):
	mkdir -p $(PARSER_OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Compile QvLib source files
$(OBJ_DIR)/%.o: $(QVLIB_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile WWW library source files
$(WWW_OBJ_DIR)/%.o: $(WWW_SRC_DIR)/%.c | $(WWW_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link QvLib library
$(LIBQV): $(QVLIB_OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Link WWW library
$(LIBWWW): $(WWW_OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Generate parser from YACC grammar
$(PARSER_YACC_C) $(PARSER_YACC_H): $(PARSER_YACC)
	cd $(PARSER_SRC_DIR) && $(YACC) -d vrml.y

# Generate lexer from LEX specification
$(PARSER_LEX_C): $(PARSER_LEX) $(PARSER_YACC_H)
	cd $(PARSER_SRC_DIR) && $(LEX) vrml.l

# Compile parser YACC output
$(PARSER_OBJ_DIR)/y.tab.o: $(PARSER_YACC_C) | $(PARSER_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(PARSER_SRC_DIR) -Wno-unused-function -c $< -o $@

# Compile parser LEX output
$(PARSER_OBJ_DIR)/lex.yy.o: $(PARSER_LEX_C) | $(PARSER_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(PARSER_SRC_DIR) -Wno-unused-function -c $< -o $@

# Compile parser interface
$(PARSER_OBJ_DIR)/QvParser.o: $(PARSER_SRCS) $(PARSER_YACC_H) | $(PARSER_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(PARSER_SRC_DIR) -c $< -o $@

# Link parser library
$(LIBPARSER): $(PARSER_OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f $(PARSER_YACC_C) $(PARSER_YACC_H) $(PARSER_LEX_C)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild

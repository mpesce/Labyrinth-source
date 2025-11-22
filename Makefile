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

# Source files (all QvLib implementations - 37 files)
SRCS = $(SRC_DIR)/QvNode.cpp \
       $(SRC_DIR)/QvFields.cpp \
       $(SRC_DIR)/QvCone.cpp \
       $(SRC_DIR)/QvCoordinate3.cpp \
       $(SRC_DIR)/QvCube.cpp \
       $(SRC_DIR)/QvCylinder.cpp \
       $(SRC_DIR)/QvDirectionalLight.cpp \
       $(SRC_DIR)/QvGroup.cpp \
       $(SRC_DIR)/QvIndexedFaceSet.cpp \
       $(SRC_DIR)/QvIndexedLineSet.cpp \
       $(SRC_DIR)/QvInfo.cpp \
       $(SRC_DIR)/QvLevelOfDetail.cpp \
       $(SRC_DIR)/QvMaterial.cpp \
       $(SRC_DIR)/QvMaterialBinding.cpp \
       $(SRC_DIR)/QvMatrixTransform.cpp \
       $(SRC_DIR)/QvNormal.cpp \
       $(SRC_DIR)/QvNormalBinding.cpp \
       $(SRC_DIR)/QvOrthographicCamera.cpp \
       $(SRC_DIR)/QvPerspectiveCamera.cpp \
       $(SRC_DIR)/QvPointLight.cpp \
       $(SRC_DIR)/QvPointSet.cpp \
       $(SRC_DIR)/QvRotation.cpp \
       $(SRC_DIR)/QvScale.cpp \
       $(SRC_DIR)/QvSeparator.cpp \
       $(SRC_DIR)/QvShapeHints.cpp \
       $(SRC_DIR)/QvSphere.cpp \
       $(SRC_DIR)/QvSpotLight.cpp \
       $(SRC_DIR)/QvSwitch.cpp \
       $(SRC_DIR)/QvTexture2.cpp \
       $(SRC_DIR)/QvTexture2Transform.cpp \
       $(SRC_DIR)/QvTextureCoordinate2.cpp \
       $(SRC_DIR)/QvTransform.cpp \
       $(SRC_DIR)/QvTransformSeparator.cpp \
       $(SRC_DIR)/QvTranslation.cpp \
       $(SRC_DIR)/QvUnknownNode.cpp \
       $(SRC_DIR)/QvWWWAnchor.cpp \
       $(SRC_DIR)/QvWWWInline.cpp

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

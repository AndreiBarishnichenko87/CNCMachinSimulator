#	Project Structure
#	.
#	|-bin
#	|-resourses
#	|-dependencies
#	|-intermediate
#	`-project(src)
#		|-modulName1
#		|	|-src
#		|	|	|-internalClass.cpp
#		|	|	|-internalClass.h
#		|	|	|-modulName.cpp
#		|	|	`- ...
#		|	`- modulName.h
#		|-modulName2
#		|	`- ...
#		|-modulName3
#		|	`- ...
#		`- main.cpp

CXX      := g++
CXXFLAGS := -Wall -std=c++17 -g

TARGET := myProject.exe
SRC_LOCATION := src

RUNTIME_DIR := $(SRC_LOCATION)
MODULES_DIRS := parserXML
MODULES_DIRS := $(MODULES_DIRS:%=$(SRC_LOCATION)/%) 

INCLUDE_DIR_DEPENDENCIES := dependencies/include
LIB_DIR_DEPENDENCIES := dependencies/lib
LIBS_FILES_DEPEND := $(notdir $(wildcard $(LIB_DIR_DEPENDENCIES)/*.a))
LIBS_FILES_DEPEND := $(subst  lib,,$(basename $(LIBS_FILES_DEPEND)))

BIN_DIR := bin
OBJ_DIR := intermediate
SRC_DIRS := $(RUNTIME_DIR) \
			$(MODULES_DIRS:%=%/src)

VPATH = $(OBJ_DIR) $(SRC_DIRS)

SRC_FILES_CPP := $(foreach DIR,$(SRC_DIRS),$(wildcard $(DIR)/*.cpp))
SRC_FILES_C := $(foreach DIR,$(SRC_DIRS),$(wildcard $(DIR)/*.c))

OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(notdir $(patsubst %.cpp,%.o,$(SRC_FILES_CPP)) ))
OBJ_FILES += $(addprefix $(OBJ_DIR)/, $(notdir $(patsubst %.c,%.o,$(SRC_FILES_C)) ))

$(TARGET): makeFolders $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o a $(OBJ_FILES) -L$(LIB_DIR_DEPENDENCIES) $(LIBS_FILES_DEPEND:%=-l%)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) -I$(<D) -I$(INCLUDE_DIR_DEPENDENCIES) $(<)

$(OBJ_DIR)/glad.o: $(RUNTIME_DIR)/glad.c
	$(CXX) $(CXXFLAGS) -c -o $(@) -I$(INCLUDE_DIR_DEPENDENCIES) $(<)

$(OBJ_DIR)/%.o: %.c

$(OBJ_DIR)/main.o: $(RUNTIME_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(MODULES_DIRS:%=-I%) $(MODULES_DIRS:%=-I%/src) \
	-I$(INCLUDE_DIR_DEPENDENCIES) $(RUNTIME_DIR)/main.cpp


makeFolders:
	if not exist "$(OBJ_DIR)" mkdir $(OBJ_DIR)
	if not exist "$(BIN_DIR)" mkdir $(BIN_DIR)

.PHONY: print clear

clear:
	del $(OBJ_DIR)\*.o
	del $(BIN_DIR)\*.exe 
	del *.exe 
	
print:
	@echo MODULES_DIR_NAME: $(MODULES_DIRS)
	@echo SRC_DIRS: $(SRC_DIRS)
	@echo SRC_FILES_CPP: $(SRC_FILES_CPP)
	@echo SRC_FILES_C: $(SRC_FILES_C)
	@echo OBJ_FILES: $(OBJ_FILES)
	@echo INCLUDE_DIR_DEPENDENCIES: $(INCLUDE_DIR_DEPENDENCIES)
	@echo LIB_DIR_DEPENDENCIES: $(LIB_DIR_DEPENDENCIES)
	@echo LIBS_FILES_DEPEND: $(LIBS_FILES_DEPEND)

	
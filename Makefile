#  .
#  |-bin
#  |-build
#  `-lib
#     |-include
#     |-lib
#  `-projects
#      |-projectName1
#      |   |-pub
#      |   |-tests
#      |   `publicHeader1.h
#      |   `ProjectName1Src.cpp
#      `-projectName2

CXX      := g++
CXXFLAGS := -Wuninitialized -std=c++17 -g
BUILD    := build
OBJ_DIR  := $(BUILD)/bin
APP_DIR  := $(BUILD)/apps
TARGET   := a
INCLUDE  := -Isrc/parserXML/pub
SRC      := $(wildcard src/runtime/*.cpp) \
						$(wildcard src/parserXML/*.cpp)

SRCOBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

BINOBJECTS := $(notdir $(SRC))
BINOBJECTS := $(BINOBJECTS:%.cpp=$(OBJ_DIR)/%.o)

all: buildFolder $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	if not exist "$(OBJ_DIR)" mkdir $(subst /,\,$(OBJ_DIR))
	$(CXX) $(CXXFLAGS) -c -D_GLIBCXX_USE_CXX11_ABI=0 $(INCLUDE) $< -o $(OBJ_DIR)/$(@F)

$(APP_DIR)/$(TARGET): $(SRCOBJECTS)
	if not exist "$(@D)" mkdir $(subst /,\,$(@D))
	$(CXX) $(CXXFLAGS) $(BINOBJECTS) -o $(TARGET).exe

buildFolder:
	if not exist "$(APP_DIR)" mkdir $(subst /,\,$(APP_DIR))
	if not exist "$(OBJ_DIR)" mkdir $(subst /,\,$(OBJ_DIR))


#=========================================================
.PHONY: clean info run

run:
	@echo "+--------------------------------------+"
	@echo "|           start project              |"
	@echo "+--------------------------------------+"
	./$(TARGET).exe

clean:
	rmdir /S /Q $(BUILD)
	
	
info:
	@echo "start makefile"
	@echo "OBJ_DIR:    $(OBJ_DIR)"
	@echo "APP_DIR:    $(APP_DIR)"
	@echo "SRC:        $(SRC)"
	@echo "SRCOBJECTS: $(SRCOBJECTS)"
	@echo "BINOBJECTS: $(BINOBJECTS)"
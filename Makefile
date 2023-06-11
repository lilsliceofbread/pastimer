PROJ_NAME := pastimer
CXX := g++
CXX_FLAGS := -g -Wall -I./inc

BUILD_DIR := ./build
OBJ_DIR := ./build/obj
SOURCES := $(wildcard ./src/*.cpp)

$(PROJ_NAME):
	mkdir -p $(BUILD_DIR)
	$(CXX) $(SOURCES) -o $(BUILD_DIR)/$(PROJ_NAME) $(CXX_FLAGS)
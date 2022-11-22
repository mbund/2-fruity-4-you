CC = gcc
CXX = g++ -std=c++17
BUILD_DIR := build
SRCS := $(wildcard src/*.cpp vendor/simulator-libraries/*.cpp vendor/simulator-libraries/*.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := vendor/simulator-libraries
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES -g -Wall

ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32 -lwinpthread -static -static-libgcc -static-libstdc++
	EXEC = game.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS = -framework OpenGL -framework Cocoa
	else
		LDFLAGS = $(shell pkg-config --libs --cflags opengl x11 glx)
	endif
	EXEC = game.out
endif

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

docs:
	doxygen

.PHONY: clean
clean:
	-rm -r $(BUILD_DIR)
	-rm $(EXEC)
	-rm -r html latex

-include $(DEPS)
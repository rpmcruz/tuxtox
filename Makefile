# BobbleBubble Makefile
# type 'make' to compile
# 'make clean' to remove compilation results

CC = g++
CXXFLAGS = -Wall -g `sdl-config --cflags`
LIBS = -lm `sdl-config --libs` -lSDL_image
SRC = bobble.cpp enemy.cpp gameloop.cpp gamemanager.cpp \
      level.cpp main.cpp player.cpp sprite.cpp menu.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = bobblebubble

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LIBS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)

# Ricardo Cruz <rick2@aiou.pt>

CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = 
TARGET = minesweeper
SRC = main.cc minesweeper.cc

ifeq ($(shell uname), Linux)
	LDFLAGS += -lGL -lGLU -lglut
endif

ifeq ($(shell uname), Darwin)
	LDFLAGS += -framework OpenGL -framework GLUT
endif

all: clean install

install:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

uninstall:
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)

style:
	clang-format -i --style=Google ./*.cc ./*.h 
	clang-format -n --style=Google ./*.cc ./*.h

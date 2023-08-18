CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lGL -lGLU -lglut

TARGET = minesweeper
SRC = main.cc minesweeper.cc

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
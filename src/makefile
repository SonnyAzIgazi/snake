CC=g++
CFLAGS=-I../include `sdl2-config --cflags --libs`
DEPS=
OBJ=main.cpp snake.cpp game.cpp

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
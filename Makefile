CC=g++

all: main

CFLAGS=-g3
LIBS=-lGLU -lSDL2 -lpthread

main: maximilian.o main.o 
	g++ -std=c++11 maximilian.o main.o -o sdl_maximilian -I /usr/local/Cellar/sdl2/2.0.10/include -L /usr/local/Cellar/sdl2/2.0.10/lib -l SDL2-2.0.0 -framework OpenGL

main.o: main.cpp
	$(CC) -std=c++11 -c main.cpp

maximilian.o: ./libs/maximilian.cpp
	$(CC) -std=c++11 -c ./libs/maximilian.cpp

clean:
	rm *.o
	rm sdl_maximilian

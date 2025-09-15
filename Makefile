all: main

main: main.cpp utils/Canvas.o
	g++ main.cpp utils/Canvas.o -o main `sdl2-config --cflags --libs` -lGLEW -lGL

utils/Canvas.o: utils/Canvas.cpp headers/utils.h
	g++ -c utils/Canvas.cpp -o utils/Canvas.o `sdl2-config --cflags`

clean:
	rm -f main utils/*.o

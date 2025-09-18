all: main

main: main.cpp utils/Window.o Scene/Scene.o
	g++ main.cpp utils/Window.o Scene/Scene.o -o main `sdl2-config --cflags --libs` -lGLEW -lGL

utils/Window.o: utils/Window.cpp headers/utils.h
	g++ -c utils/Window.cpp -o utils/Window.o `sdl2-config --cflags`
Scene/Scene.o: Scene/Scene.cpp headers/Scene.h
	g++ -c Scene/Scene.cpp -o Scene/Scene.o `sdl2-config --cflags` -lGLEW -lGL

clean:
	rm -f main utils/*.o

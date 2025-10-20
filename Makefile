all: main

main: main.cpp build/Vec4.o build/RGB.o build/Window.o build/Ray.o build/Sphere.o build/Flat.o build/Cylinder.o build/Scene.o build/Light.o
	mkdir -p build
	g++ main.cpp build/Vec4.o build/RGB.o build/Window.o build/Ray.o build/Sphere.o build/Flat.o build/Cylinder.o build/Scene.o build/Light.o -o main `sdl2-config --cflags --libs` -lGLEW -lGL

build/Vec4.o: utils/Vec4.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/Vec4.cpp -o build/Vec4.o `sdl2-config --cflags`
build/RGB.o: utils/RGB.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/RGB.cpp -o build/RGB.o `sdl2-config --cflags`

build/Window.o: utils/Window.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/Window.cpp -o build/Window.o `sdl2-config --cflags`

build/Ray.o: Scene/Ray.cpp headers/Ray.h
	mkdir -p build
	g++ -c Scene/Ray.cpp -o build/Ray.o `sdl2-config --cflags`

build/Sphere.o: Scene/Sphere.cpp headers/Sphere.h
	mkdir -p build
	g++ -c Scene/Sphere.cpp -o build/Sphere.o `sdl2-config --cflags`
build/Flat.o: Scene/Flat.cpp headers/Flat.h
	mkdir -p build
	g++ -c Scene/Flat.cpp -o build/Flat.o `sdl2-config --cflags` 

build/Cylinder.o: Scene/Cylinder.cpp headers/Cylinder.h
	mkdir -p build
	g++ -c Scene/Cylinder.cpp -o build/Cylinder.o `sdl2-config --cflags`
	
build/Scene.o: Scene/Scene.cpp headers/Scene.h
	mkdir -p build
	g++ -c Scene/Scene.cpp -o build/Scene.o `sdl2-config --cflags` -lGLEW -lGL
build/Light.o: Scene/Light.cpp headers/Light.h
	mkdir -p build
	g++ -c Scene/Light.cpp -o build/Light.o `sdl2-config --cflags`

clean:
	rm -rf build main

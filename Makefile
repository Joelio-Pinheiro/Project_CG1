all: main

main: main.cpp build/Texture.o build/Vec4.o build/AABB.o build/RGB.o build/Window.o build/Ray.o build/Sphere.o build/Flat.o build/Cylinder.o build/Cone.o build/Triangle.o build/Mesh.o build/Camera.o build/Scene.o build/Light.o build/imgui.o build/imgui_draw.o build/imgui_widgets.o build/imgui_tables.o build/imgui_impl_sdl2.o build/imgui_impl_sdlrenderer2.o
	mkdir -p build
	g++ main.cpp build/Texture.o build/Vec4.o build/AABB.o build/RGB.o build/Window.o build/Ray.o build/Sphere.o build/Flat.o build/Cylinder.o build/Cone.o build/Triangle.o build/Mesh.o build/Camera.o build/Scene.o build/Light.o build/imgui.o build/imgui_draw.o build/imgui_widgets.o build/imgui_tables.o build/imgui_impl_sdl2.o build/imgui_impl_sdlrenderer2.o -o main `sdl2-config --cflags --libs` -lSDL2_image -Iimgui -Iimgui/backends

build/Texture.o: Scene/Texture.cpp headers/Texture.h
	mkdir -p build
	g++ -c Scene/Texture.cpp -o build/Texture.o `sdl2-config --cflags` -Iimgui -Iimgui/backends
build/Vec4.o: utils/Vec4.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/Vec4.cpp -o build/Vec4.o `sdl2-config --cflags` -Iimgui -Iimgui/backends
build/AABB.o: utils/AABB.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/AABB.cpp -o build/AABB.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/RGB.o: utils/RGB.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/RGB.cpp -o build/RGB.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Window.o: utils/Window.cpp headers/utils.h
	mkdir -p build
	g++ -c utils/Window.cpp -o build/Window.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Ray.o: Scene/Ray.cpp headers/Ray.h
	mkdir -p build
	g++ -c Scene/Ray.cpp -o build/Ray.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Sphere.o: Scene/Sphere.cpp headers/Sphere.h
	mkdir -p build
	g++ -c Scene/Sphere.cpp -o build/Sphere.o `sdl2-config --cflags` -Iimgui -Iimgui/backends
build/Flat.o: Scene/Flat.cpp headers/Flat.h
	mkdir -p build
	g++ -c Scene/Flat.cpp -o build/Flat.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Cylinder.o: Scene/Cylinder.cpp headers/Cylinder.h
	mkdir -p build
	g++ -c Scene/Cylinder.cpp -o build/Cylinder.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Cone.o: Scene/Cone.cpp headers/Cone.h
	mkdir -p build
	g++ -c Scene/Cone.cpp -o build/Cone.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Triangle.o: Scene/Triangle.cpp headers/Triangle.h
	mkdir -p build
	g++ -c Scene/Triangle.cpp -o build/Triangle.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Mesh.o: Scene/Mesh.cpp headers/Mesh.h
	mkdir -p build
	g++ -c Scene/Mesh.cpp -o build/Mesh.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Camera.o: Scene/Camera.cpp headers/Camera.h
	mkdir -p build
	g++ -c Scene/Camera.cpp -o build/Camera.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/Scene.o: Scene/Scene.cpp headers/Scene.h
	mkdir -p build
	g++ -c Scene/Scene.cpp -o build/Scene.o `sdl2-config --cflags` -Iimgui -Iimgui/backends
build/Light.o: Scene/Light.cpp headers/Light.h
	mkdir -p build
	g++ -c Scene/Light.cpp -o build/Light.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/imgui.o: imgui/imgui.cpp
	g++ -c imgui/imgui.cpp -o build/imgui.o -Iimgui -Iimgui/backends

build/imgui_draw.o: imgui/imgui_draw.cpp
	g++ -c imgui/imgui_draw.cpp -o build/imgui_draw.o -Iimgui -Iimgui/backends

build/imgui_widgets.o: imgui/imgui_widgets.cpp
	g++ -c imgui/imgui_widgets.cpp -o build/imgui_widgets.o -Iimgui -Iimgui/backends

build/imgui_tables.o: imgui/imgui_tables.cpp
	g++ -c imgui/imgui_tables.cpp -o build/imgui_tables.o -Iimgui -Iimgui/backends

build/imgui_impl_sdl2.o: imgui/backends/imgui_impl_sdl2.cpp
	g++ -c imgui/backends/imgui_impl_sdl2.cpp -o build/imgui_impl_sdl2.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

build/imgui_impl_sdlrenderer2.o: imgui/backends/imgui_impl_sdlrenderer2.cpp
	g++ -c imgui/backends/imgui_impl_sdlrenderer2.cpp -o build/imgui_impl_sdlrenderer2.o `sdl2-config --cflags` -Iimgui -Iimgui/backends

clean:
	rm -rf build main

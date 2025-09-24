#include "../headers/Scene.h"
#include "../headers/Sphere.h"
#include "../headers/utils.h"
#include <SDL.h>
#include <GL/glew.h>

Scene::Scene(float width, float height, float DWindow, utils::window *window){
    this->WIDTH = width;
    this->HEIGHT = height;
    this->window = window;
    this->DWindow = DWindow;
    this->setObserverPosition(0.0f, 0.0f, 0.0f);

    this->window->setPosition(0, 0, this->DWindow); // Supose Dwindow is -5.0f

    // Create a ball and add it to the scene
    float radius = 1.0f;
    float Zcenter = -(this->DWindow + radius);
    Sphere *sphere1 = new Sphere(radius, 0.0f, 0.0f, Zcenter);
    this->spheres.push_back(sphere1);
}

void Scene::setObserverPosition(float x, float y, float z) {
    this->observerPosition.x = x;
    this->observerPosition.y = y;
    this->observerPosition.z = z;
}
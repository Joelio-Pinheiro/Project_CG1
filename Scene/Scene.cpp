#include "../headers/Scene.h"
#include "../headers/utils.h"
#include <SDL.h>
#include <GL/glew.h>

Scene::Scene(int width, int height, float DWindow, utils::window *window){
    this->WIDTH = width;
    this->HEIGHT = height;
    this->window = window;
    this->DWindow = DWindow;
    // this->DWindow = -5.0f;
    setObserverPosition(0.0f, 0.0f, 0.0f);

    this->window->setPosition(0, 0, this->DWindow);
}

void Scene::setObserverPosition(float x, float y, float z) {
    this->observerPosition.x = x;
    this->observerPosition.y = y;
    this->observerPosition.z = z;
}
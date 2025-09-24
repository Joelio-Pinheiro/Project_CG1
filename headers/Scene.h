#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include <vector>

class Scene {
    private:
        float WIDTH;
        float HEIGHT;
        float DWindow;
        utils::window *window;
        utils::Vec4 observerPosition;
        std::vector<Sphere*> spheres;
    public:
        Scene(float width, float height, float DWindow, utils::window *window);
        void setObserverPosition(float x, float y, float z);
};

#endif // SCENE_H
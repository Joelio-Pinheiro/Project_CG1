#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"


class Scene {
    private:
        int WIDTH;
        int HEIGHT;
        float DWindow;
        utils::window *window;
        utils::Position observerPosition;
    public:
        Scene(int width, int height, float DWindow, utils::window *window);
        void setObserverPosition(float x, float y, float z);
};

#endif // SCENE_H
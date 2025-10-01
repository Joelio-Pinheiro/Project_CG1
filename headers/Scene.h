#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Ray.h"
#include "../headers/Light.h"
#include <vector>

class Scene {
    private:
        float WIDTH;
        float HEIGHT;
        float DWindow;
        int nRow;
        int nCol;
        utils::window *window;
        utils::Vec4 observerPosition;
        utils::RGB ambientLight = {0.1f, 0.1f, 0.1f};
        std::vector<Sphere*> spheres;
        std::vector<Light*> lights;
    public:
        Scene(float width, float height, float DWindow, int nRow, int nCol, utils::window *window);
        void setObserverPosition(float x, float y, float z);
        void render();
    private:
        std::vector<SDL_Color> traceRays();
        void drawCanvas(const std::vector<SDL_Color>& canvas);
};

#endif // SCENE_H
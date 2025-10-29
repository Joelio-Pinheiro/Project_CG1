#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Flat.h"
#include "../headers/Cylinder.h"
#include "../headers/Cone.h"
#include "../headers/Triangle.h"
#include "../headers/Mesh.h"
#include "../headers/Ray.h"
#include "../headers/Light.h"
#include <vector>

class Light;
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
        std::vector<Flat*> flats;
        std::vector<Cylinder*> cylinders;
        std::vector<Cone*> cones;
        std::vector<Triangle*> triangles;
        std::vector<Mesh*> meshes;
        std::vector<Light*> lights;
    public:
        Scene(float width, float height, float DWindow, int nRow, int nCol, utils::window *window);
        void setObserverPosition(float x, float y, float z);

        void setAmbientLight(float r, float g, float b) { this->ambientLight = utils::RGB(r, g, b); }
        utils::RGB getAmbientLight() const { return this->ambientLight; }
        std::vector<Sphere*> getSpheres() const { return this->spheres; }
        std::vector<Flat*> getFlats() const { return this->flats; }
        std::vector<Cylinder*> getCylinders() const { return this->cylinders; }
        std::vector<Cone*> getCones() const { return this->cones; }
        std::vector<Triangle*> getTriangles() const { return this->triangles; }
        std::vector<Mesh*> getMeshes() const { return this->meshes; }

        void incrementObserverZ(float z) { this->observerPosition.z += z; }
        void incrementObserverX(float x) { this->observerPosition.x += x; }
        void incrementObserverY(float y) { this->observerPosition.y += y; }
        void decrementObserverZ(float z) { this->observerPosition.z -= z; }
        void decrementObserverX(float x) { this->observerPosition.x -= x; }
        void decrementObserverY(float y) { this->observerPosition.y -= y; }
        utils::Vec4 getObserverPosition() const { return this->observerPosition; }
        void render();
    private:
        std::vector<SDL_Color> traceRays();
        void drawCanvas(const std::vector<SDL_Color>& canvas);
};

#endif // SCENE_H
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
#include "../headers/Texture.h"
#include "../headers/Camera.h"
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
        Camera *camera;
        utils::RGB ambientLight = {0.1f, 0.1f, 0.1f};

        // objetos
        std::vector<Sphere*> spheres;
        std::vector<Flat*> flats;
        std::vector<Cylinder*> cylinders;
        std::vector<Cone*> cones;
        std::vector<Triangle*> triangles;
        std::vector<Mesh*> meshes;
        std::vector<Light*> lights;

        SDL_Texture* canvasTexture = nullptr;
        SDL_PixelFormat* canvasFormat = nullptr;
        int texW = 0, texH = 0;

        //interatividade
        bool mouseCaptured = false;
        int lasMouseX = 0;
        int lastMouseY = 0;
        bool ignoreNextMouseMotion = false;
        float mouseSensitivity = 0.12f;
        float moveSpeed = 0.1f;
        float zoomSpeed = 0.5f;
        int speedRender = 3;
        bool dirty = true;
    public:
        Scene(float width, float height, float DWindow, int nRow, int nCol, utils::window *window);
        void setCamera(Camera* cam) { this->camera = cam; }

        void setAmbientLight(float r, float g, float b) { this->ambientLight = utils::RGB(r, g, b); }
        utils::RGB getAmbientLight() const { return this->ambientLight; }
        std::vector<Sphere*> getSpheres() const { return this->spheres; }
        std::vector<Flat*> getFlats() const { return this->flats; }
        std::vector<Cylinder*> getCylinders() const { return this->cylinders; }
        std::vector<Cone*> getCones() const { return this->cones; }
        std::vector<Triangle*> getTriangles() const { return this->triangles; }
        std::vector<Mesh*> getMeshes() const { return this->meshes; }
        // utils
        int getNRow() const { return this->nRow; }
        int getNCol() const { return this->nCol; }
        SDL_Texture* getCanvasTexture() const { return canvasTexture; }
        
        
        // Interatividade
        void pick(int mx, int my);
        void handleEvent(const SDL_Event& e);
        void handleKeyboard(const Uint8* keys);
        Camera* getCamera() const { return this->camera; }
        int getSpeedRender() const { return this->speedRender; }
        void setSpeedRender(int sr) { this->speedRender = sr; }
        void markDirty() { dirty = true; }
        void clearDirty() { dirty = false; }
        bool isDirty() const { return dirty; }

        void render();
        ~Scene() {
            if (canvasTexture) SDL_DestroyTexture(canvasTexture);
            if (canvasFormat) SDL_FreeFormat(canvasFormat);
        };
    private:
        std::vector<SDL_Color> traceRays();
        void drawCanvas(const std::vector<SDL_Color>& canvas);
        void ensureTexture(int w, int h);
};

#endif // SCENE_H
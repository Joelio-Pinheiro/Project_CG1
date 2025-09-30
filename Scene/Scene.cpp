#include "../headers/Scene.h"
#include "../headers/Sphere.h"
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Light.h"
#include <SDL.h>
#include <GL/glew.h>

Scene::Scene(float width, float height, float DWindow, int nRow, int nCol, utils::window *window){
    this->WIDTH = width;
    this->HEIGHT = height;
    this->window = window;
    this->DWindow = DWindow;
    this->nRow = nRow;
    this->nCol = nCol;
    this->setObserverPosition(0.0f, 0.0f, 200.0f);

    this->window->setPosition(0, 0, this->DWindow); 

    // Criar esferas na cena
    float radius = 2.9f;
    float Zcenter = this->DWindow + radius; // Assumindo que DWindow é negativo
    Sphere *sphere1 = new Sphere(radius, -4.0f, 0.0f, 0.0f);
    sphere1->setcolors(255, 0, 0); // Red color
    this->spheres.push_back(sphere1);

    Sphere *sphere2 = new Sphere(radius, 4.0f, 0.5f, 0.0f);
    sphere2->setcolors(0, 255, 0); 
    this->spheres.push_back(sphere2);

    Light *light1 = new Light(50.0f, 0.0f, 0.0f, utils::Vec4::Vector(1.0f, 1.0f, 1.0f));
    this->lights.push_back(light1);

}

void Scene::setObserverPosition(float x, float y, float z) {
    this->observerPosition.x = x;
    this->observerPosition.y = y;
    this->observerPosition.z = z;
}

std::vector<SDL_Color> Scene::traceRays() {
    float Dx = this->WIDTH / this->nCol;
    float Dy = this->HEIGHT / this->nRow;

    SDL_Color bgColor  = {100, 100, 100, 255};

    std::vector<SDL_Color> canvas(this->nRow * this->nCol, bgColor); // armazena todos os pixels com a cor de fundo

    float Z = this->DWindow; // Z coordenada da janela
    for (int l = 0; l < this->nRow; l++) {
        float Y = this->HEIGHT/2 - Dy/2 - l*Dy; // Y coordenada do pixel
        for(int c = 0; c < this->nCol; c++) {
            float X = -this->WIDTH/2 + Dx/2 + c*Dx; // X coordenada do pixel

            utils::Vec4 pixel = utils::Vec4::Point(X, Y, Z);
            utils::Vec4 dir   = (pixel - this->observerPosition);
            Ray ray(this->observerPosition, dir);

            SDL_Color pixelColor = bgColor;

            HitInfo closestHit;
            closestHit.hit = false;
            closestHit.t = std::numeric_limits<float>::max();
            SDL_Color hitColor = bgColor;
            
            for (Sphere* sphere : this->spheres) {
                HitInfo hitInfo = sphere->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;
                    // hitColor = sphere->getColors();
                    utils::Vec4 totalLight = utils::Vec4::Point(0, 0, 0);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, sphere, ray.getDirection());
                    }
                    hitColor = {static_cast<Uint8>(totalLight.x * 255), static_cast<Uint8>(totalLight.y * 255), static_cast<Uint8>(totalLight.z * 255), 255};
                }
            }
            if (closestHit.hit) {
                pixelColor = hitColor;
            }
            canvas[l * this->nCol + c] = pixelColor;
        }
    }

    return canvas;
}

void Scene::drawCanvas(const std::vector<SDL_Color>& canvas) {
    SDL_Renderer* renderer = window->getRenderer();

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        this->nCol, this->nRow
    );

    void* pixels;
    int pitch;
    SDL_LockTexture(texture, nullptr, &pixels, &pitch);

    Uint32* dst = static_cast<Uint32*>(pixels);
    for (int i = 0; i < nRow * nCol; i++) {
        SDL_Color c = canvas[i];
        dst[i] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), c.r, c.g, c.b, c.a);
    }


    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
}

void Scene::render() {
    std::vector<SDL_Color> canvas = this->traceRays();
    this->drawCanvas(canvas);
}

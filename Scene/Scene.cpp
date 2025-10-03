#include "../headers/Scene.h"
#include "../headers/Sphere.h"
#include "../headers/Flat.h"
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
    this->ambientLight = utils::RGB(0.2f, 0.2f, 0.2f);

    // Criar esferas na cena
    float radius = 2.9f;
    float Zcenter = this->DWindow + radius; // Assumindo que DWindow é negativo
    Sphere *sphere1 = new Sphere(radius, -4.0f, 0.0f, 0.0f);
    sphere1->setDiffuse(1.0f, 0.0f, 0.0f); // Red color
    sphere1->setSpecular(1.0f, 1.0f, 1.0f);
    this->spheres.push_back(sphere1);

    Sphere *sphere2 = new Sphere(radius, 6.0f, -1.5f, 0.0f);
    sphere2->setDiffuse(0.0f, 1.0f, 0.0f);
    sphere2->setSpecular(1.0f, 1.0f, 1.0f);
    this->spheres.push_back(sphere2);

    Flat *floor = new Flat(utils::Vec4::Point(0.0f, -3.0f, 0.0f), utils::Vec4::Vector(0.0f, 1.0f, 0.0f));
    floor->setDiffuse(0.5f, 0.5f, 0.5f);
    floor->setSpecular(1.0f, 1.0f, 1.0f);
    this->flats.push_back(floor);

    Light *light1 = new Light(50.0f, 10.0f, 0.0f, utils::RGB(1.0f, 1.0f, 1.0f), this->ambientLight);
    Light *light2 = new Light(-40.0f, 15.0f, 0.0f, utils::RGB(1.0f, 1.0f, 1.0f), this->ambientLight);
    this->lights.push_back(light1);
    this->lights.push_back(light2);
}

void Scene::setObserverPosition(float x, float y, float z) {
    this->observerPosition.x = x;
    this->observerPosition.y = y;
    this->observerPosition.z = z;
}

std::vector<SDL_Color> Scene::traceRays() {
    float Dx = this->WIDTH / this->nCol;
    float Dy = this->HEIGHT / this->nRow;

    utils::RGB bgColor  = {0.1f, 0.1f, 0.1f};

    std::vector<SDL_Color> canvas(this->nRow * this->nCol, bgColor.toSDLColor()); // armazena todos os pixels com a cor de fundo

    float Z = this->DWindow; // Z coordenada da janela
    for (int l = 0; l < this->nRow; l++) {
        float Y = this->HEIGHT/2 - Dy/2 - l*Dy; // Y coordenada do pixel
        for(int c = 0; c < this->nCol; c++) {
            float X = -this->WIDTH/2 + Dx/2 + c*Dx; // X coordenada do pixel

            utils::Vec4 pixel = utils::Vec4::Point(X, Y, Z);
            utils::Vec4 dir   = (pixel - this->observerPosition);
            Ray ray(this->observerPosition, dir);

            utils::RGB pixelColor = bgColor;

            HitInfo closestHit;
            closestHit.hit = false;
            closestHit.t = std::numeric_limits<float>::max();
            utils::RGB hitColor = bgColor;
            
            for (Sphere* sphere : this->spheres) {
                HitInfo hitInfo = sphere->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;

                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, sphere, &this->spheres, ray.getDirection());
                    }
                    hitColor = totalLight;
                }
            }
            for (Flat* flat : this->flats) {
                HitInfo hitInfo = flat->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;

                    //utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    // for(Light* light : this->lights) {
                    //     // Note: Flat does not have specular component in this example
                    //     //totalLight = totalLight + light->ComputeLighting(hitInfo, nullptr, &this->spheres, ray.getDirection());
                    // }
                    hitColor = flat->getDiffuse();
                }
            }
            if (closestHit.hit) {
                pixelColor = hitColor;
            }
            canvas[l * this->nCol + c] = pixelColor.toSDLColor();
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

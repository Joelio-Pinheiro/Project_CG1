#include "../headers/Scene.h"
#include "../headers/Sphere.h"
#include "../headers/Flat.h"
#include "../headers/Cylinder.h"
#include "../headers/Cone.h"
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
    this->setObserverPosition(0.0f, 0.0f, 0.0f);

    this->window->setPosition(0, 0, this->DWindow); 
    this->setAmbientLight(0.3f, 0.3f, 0.3f);

    float radius = 40.0f;
    float Zcenter = this->DWindow + radius; // Assumindo que DWindow é negativo
    Sphere *sphere1 = new Sphere(radius, 0.0f, 0.0f, -100.0f);
    sphere1->setDiffuse(0.7f, 0.2f, 0.2f);
    sphere1->setSpecular(0.7f, 0.2f, 0.2f);
    sphere1->setShininess(10.0f);
    this->spheres.push_back(sphere1);
    
    Flat *floor = new Flat(
        utils::Vec4::Point(0.0f, -radius, 0.0f), 
        utils::Vec4::Vector(0.0f, 1.0f, 0.0f));
    floor->setDiffuse(0.2f, 0.7f, 0.2f);
    floor->setSpecular(0.0f, 0.0f, 0.0f);
    floor->setShininess(1.0f);
    this->flats.push_back(floor);
    
    Flat *back = new Flat(
        utils::Vec4::Point(0.0f, 0.0f, -200.0f), 
        utils::Vec4::Vector(0.0f, 0.0f, 1.0f));
    back->setDiffuse(0.3f, 0.3f, 0.7f);
    back->setSpecular(0.0f, 0.0f, 0.0f);
    back->setShininess(1.0f);
    this->flats.push_back(back);
    
    // Vetor direção (normalizado)
    utils::Vec4 d_cil(-1/sqrt(3), 1/sqrt(3), -1/sqrt(3));

    // ---- CILINDRO ----
    Cylinder *cylinder1 = new Cylinder(
        radius / 3.0f,        
        radius * 3.0f,        
        utils::Vec4::Point(0.0f, 0.0f, -100.0f), 
        d_cil,                
        false);
    cylinder1->setDiffuse(0.2f, 0.3f, 0.8f);
    cylinder1->setSpecular(0.2f, 0.3f, 0.8f);
    this->cylinders.push_back(cylinder1);

    // ---- CONE ----
    utils::Vec4 coneBaseCenter = utils::Vec4::Point(0.0f, 0.0f, -100.0f) + d_cil * (3.0f * radius);

    // Raio e altura do cone
    float coneBaseRadius = 1.5f * radius;
    float coneHeight = coneBaseRadius / 3.0f; // (1/3)*raio base

    Cone *cone1 = new Cone(
        coneBaseRadius,
        coneHeight,
        coneBaseCenter,
        d_cil,
        true);
    cone1->setDiffuse(0.8f, 0.3f, 0.2f);
    cone1->setSpecular(0.8f, 0.3f, 0.2f);
    this->cones.push_back(cone1);


    Light *light1 = new Light(
        0.0f, 60.0f, -30.0f, 
        utils::RGB(0.7f, 0.7f, 0.7f), this);
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

            utils::HitInfo closestHit;
            closestHit.hit = false;
            closestHit.t = std::numeric_limits<float>::max();
            utils::RGB hitColor = bgColor;
            
            for (Sphere* sphere : this->spheres) {
                utils::HitInfo hitInfo = sphere->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;

                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, sphere->getMaterial(), ray.getDirection());
                    }
                    hitColor = totalLight;
                }
            }
            for (Cylinder* cylinder : this->cylinders) {
                utils::HitInfo hitInfo = cylinder->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;

                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, cylinder->getMaterial(), ray.getDirection());
                    }
                    hitColor = totalLight;
                }
            }
            for (Cone* cone : this->cones) {
                utils::HitInfo hitInfo = cone->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;

                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, cone->getMaterial(), ray.getDirection());
                    }
                    hitColor = totalLight;
                }
            }
            for (Flat* flat : this->flats) {
                utils::HitInfo hitInfo = flat->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;

                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, flat->getMaterial(), ray.getDirection());
                    }
                    hitColor = totalLight;
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

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

    this->window->setPosition(0, 0, this->DWindow); 
    this->setCamera(new Camera());
    this->setAmbientLight(0.3f, 0.3f, 0.3f);
    float K = 7.0f;
    utils::Vec4 C2, C3, C4, C5;


    // Plano OBJ1
    Flat *floor = new Flat(
        utils::Vec4::Point(0.0f, 0.0f, 0.0f),
        utils::Vec4::Vector(0.0f, 0.0f, 1.0f)
    );
    floor->setDiffuse(0.5f, 0.25f, 0.5f);
    floor->setSpecular(0.5f, 0.25f, 0.5f);
    floor->setShininess(2.0f);
    this->flats.push_back(floor);


    // Anel Cilindrico OBJ2
    C2  = utils::Vec4::Point(100 - K, 200 + K, 0); 
    float R2 = 10.0f + K;
    float H2 = 5.0f + 20.0f * K;

    Cylinder *cylinder = new Cylinder(
        1.0f * R2,
        1.0f * H2,
        C2,
        utils::Vec4::Vector(0.0f, 1.0f, 0.0f),
        true
    );
    cylinder->setDiffuse(0.5f, 0.25f, 0.5f);
    cylinder->setSpecular(0.5f, 0.25f, 0.5f);
    cylinder->setShininess(2.0f);
    this->cylinders.push_back(cylinder);

    // Esfera OBJ3
    C3 = utils::Vec4::Point(100 - K, 200 + K, 20 * K - R2/4);
    float R3 = 20 + K; 
    Sphere *sphere1 = new Sphere(
        1.0f * R3,
        C3.x, C3.y, C3.z
    );
    sphere1->setDiffuse(0.5f, 0.25f, 0.5f);
    sphere1->setSpecular(0.5f, 0.25f, 0.5f);
    sphere1->setShininess(2.0f);
    this->spheres.push_back(sphere1);    

    // Cone de Base aberta OBJ4
    C4 = utils::Vec4::Point(200 + K, 100 - K, 0);
    float R4 = 30.0f + K;
    float H4 = 10.0f + 50.0f * K;
    Cone *cone = new Cone(
        1.0f * R4,
        1.0f * H4,
        C4,
        utils::Vec4::Vector(1.0f, 0.0f, 0.0f),
        false
    );
    cone->setDiffuse(0.5f, 0.25f, 0.5f);
    cone->setSpecular(0.5f, 0.25f, 0.5f);
    cone->setShininess(2.0f);
    this->cones.push_back(cone);

    // Pirâmide reta de base quadrada OBJ5
    C5 = utils::Vec4::Point(200 + K, 200 + K, 0);
    float L5 = 5.0f ;
    float H5 = 20.0f + 20.0f * K;
    float delta = M_PI / 5; // para girar a pirâmide um pouco

    std::vector<utils::Vec4> verts = {
        utils::Vec4::Point(-0.5f, -0.5f, 0.0f), // B1 - 0
        utils::Vec4::Point( 0.5f, -0.5f, 0.0f), // B2 - 1
        utils::Vec4::Point( 0.5f,  0.5f, 0.0f), // B3 - 2
        utils::Vec4::Point(-0.5f,  0.5f, 0.0f), // B4 - 3
        utils::Vec4::Point( 0.0f,  0.0f, 1.0f)  // V  - 4
    };
    

    // Índices dos triângulos
    std::vector<unsigned int> indices = {
        // Base (2 triângulos)
        0, 1, 2,
        0, 2, 3,

        // Faces laterais (4 triângulos)
        4, 0, 1,
        4, 1, 2,
        4, 2, 3,
        4, 3, 0
    };
    Mesh *pyramid = new Mesh(verts, indices);
    pyramid->setDiffuse(0.5f, 0.25f, 0.5f);
    pyramid->setSpecular(0.5f, 0.25f, 0.5f);
    pyramid->setShininess(2.0f);
    this->meshes.push_back(pyramid);

    utils::Vec4 p1 = utils::Vec4::Point(-0.5f,  0.5f, 0.0f);
    p1.x *= L5;
    p1.y *= L5;
    p1.z *= H5;


    float rad = delta * (M_PI / 180.0f);
    float cosA = cos(rad);
    float sinA = sin(rad);

    // Rotação no P1
    float x1 = p1.x * cosA - p1.y * sinA;
    float y1 = p1.x * sinA + p1.y * cosA;
    p1.x = x1;
    p1.y = y1;

    //translation
    p1 = p1 + C5;
    std::cout << "Piramide Vertice 1 escalado: "; p1.print();

    // Transformações dos objetos para suas posições corretas
    // OBJ5
    pyramid->scale(L5, L5, H5, utils::Vec4::Point(0.0f, 0.0f, 0.0f));
    pyramid->rotationZ(delta, utils::Vec4::Point(0.0f, 0.0f, 0.0f));
    pyramid->translation(C5);

    Light *mainLight = new Light(
        300.0f, 300.0f, 1000.0f,
        utils::RGB(1.0f, 1.0f, 1.0f),
        this
    );
    this->lights.push_back(mainLight);

}

std::vector<SDL_Color> Scene::traceRays() {
    // std::cout << "Camera Eye: "; camera->getEye().print();
    // std::cout << "Camera Forward: "; camera->getForward().print();
    // std::cout << "Camera Up: "; camera->getUp().print();
    // std::cout << "Camera Right: "; camera->getRight().print();
    // std::cout << "DWindow: " << this->DWindow << std::endl;

    float Dx = this->WIDTH / this->nCol;
    float Dy = this->HEIGHT / this->nRow;
    
    utils::RGB bgColor  = {0.1f, 0.1f, 0.1f};
    
    std::vector<SDL_Color> canvas(this->nRow * this->nCol, bgColor.toSDLColor()); // armazena todos os pixels com a cor de fundo
    
    float Z = this->DWindow; // Z coordenada da janela
    for (int l = 0; l < this->nRow; l++) {
        float Y = this->HEIGHT/2 - Dy/2 - l*Dy; // Y coordenada do pixel
        for(int c = 0; c < this->nCol; c++) {
            float X = -this->WIDTH/2 + Dx/2 + c*Dx; // X coordenada do pixel
            
            
            utils::Vec4 pixel =
                camera->getEye()
                + camera->getForward() * Z     // Z positivo
                + camera->getRight()   * X
                + camera->getUp()      * Y;

            utils::Vec4 dir = (pixel - camera->getEye()).normalize();
            Ray ray(camera->getEye(), dir);
            
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
            for (Triangle* triangle : this->triangles) {
                utils::HitInfo hitInfo = triangle->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;
                    
                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, triangle->getMaterial(), ray.getDirection());
                    }
                    hitColor = totalLight;
                }
            }
            for (Mesh* mesh : this->meshes) {
                utils::HitInfo hitInfo = mesh->intersects(ray);
                if (hitInfo.hit && hitInfo.t < closestHit.t) {
                    closestHit = hitInfo;
                    
                    utils::RGB totalLight = utils::RGB(0.0f, 0.0f, 0.0f);
                    for(Light* light : this->lights) {
                        totalLight = totalLight + light->ComputeLighting(hitInfo, mesh->getMaterial(), ray.getDirection());
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
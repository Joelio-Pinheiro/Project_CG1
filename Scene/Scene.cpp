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



    // Chão
    Flat *floor = new Flat(
        utils::Vec4::Point(0.0f, -1.5f, 0.0f),
        utils::Vec4::Vector(0.0f, 1.0f, 0.0f)
    );
    Texture* floorTexture = new Texture("models/madeira.png");
    utils::Material floorMat;

    floorMat.setTexture(floorTexture);
    floorMat.setShininess(2.0f);
    floor->setMaterial(floorMat);
    this->flats.push_back(floor);
    
    // Parede lateral direita
    Flat *rightWall = new Flat(
        utils::Vec4::Point(2.0f, -1.5f, 0.0f),
        utils::Vec4::Vector(-1.0f, 0.0f, 0.0f)
    );
    rightWall->setDiffuse(0.686f, 0.933f, 0.933f);
    rightWall->setSpecular(0.686f, 0.933f, 0.933f);
    rightWall->setShininess(2.0f);
    this->flats.push_back(rightWall);

    // PAREDE 
    Flat *frontWall = new Flat(
        utils::Vec4::Point(0.0f, -1.5f, 2.0f),
        utils::Vec4::Vector(0.0f, 0.0f, -1.0f)
    );
    frontWall->setDiffuse(0.686f, 0.933f, 0.933f);
    frontWall->setSpecular(0.686f, 0.933f, 0.933f);
    this->flats.push_back(frontWall);

    // PAREDE LATERAL ESQUERDA
    Flat *leftWall = new Flat(
        utils::Vec4::Point(-2.0f, -1.5f, 0.0f),
        utils::Vec4::Vector(1.0f, 0.0f, 0.0f)
    );
    leftWall->setDiffuse(0.686f, 0.933f, 0.933f);
    leftWall->setSpecular(0.686f, 0.933f, 0.933f);
    this->flats.push_back(leftWall);

    // TETO
    Flat *ceiling = new Flat(
        utils::Vec4::Point(0.0f, 1.5f, 0.0f),
        utils::Vec4::Vector(0.0f, -1.0f, 0.0f)
    );
    ceiling->setDiffuse(0.933f, 0.933f, 0.933f);
    ceiling->setSpecular(0.933f, 0.933f, 0.933f);
    this->flats.push_back(ceiling);
    // CUBO
    utils::Vec4 baseCenter = utils::Vec4::Point(0.0f, -1.5f, -1.65f);
    float a = 0.5f; // metade da aresta basica

    std::vector<utils::Vec4> cubeVerts = {
        // base (y = 0)
        utils::Vec4::Point(-a, 0.0f, -a),
        utils::Vec4::Point(a, 0.0f, -a),
        utils::Vec4::Point(a, 0.0f, a),
        utils::Vec4::Point(-a, 0.0f, a),

        // topo (y = 1 porque aresta=1 no espaço local)
        utils::Vec4::Point(-a, 1.0f, -a),
        utils::Vec4::Point(a, 1.0f, -a),
        utils::Vec4::Point(a, 1.0f, a),
        utils::Vec4::Point(-a, 1.0f, a)
    };
    // indices dos triângulos
    std::vector<unsigned int> cubeIndices = {
        0,1,2,  0,2,3,
        4,5,6,  4,6,7,
        0,1,5,  0,5,4,
        3,2,6,  3,6,7,
        0,3,7,  0,7,4,
        1,2,6,  1,6,5
    };
    
    Mesh *cube = new Mesh(cubeVerts, cubeIndices);
    utils::Material blueMat;
    blueMat.diffuse = {1.0f, 0.078f, 0.576f};
    blueMat.specular = {1.0f, 0.078f, 0.576f};
    cube->setMaterial(blueMat);

    // Transformações no cubo
    utils::Vec4 modelCenter = utils::Vec4::Point(0.0f, 0.5f, 0.0f);
    cube->scale(1.2f, modelCenter); 
    cube->shear(0.3f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            modelCenter);      
    utils::Vec4 worldPos = utils::Vec4::Vector(0.0f, -2.5f, -1.65f - 1.5f); 
    cube->translation(worldPos);
    cube->reflection(
        utils::Vec4::Point(0.0f, -1.5f, 0.0f),
        utils::Vec4::Vector(0.0f, 1.0f, 0.0f) 
    );
    // cube->rotationY(25.0f, modelCenter + worldPos);
    // cube->rotationX(15.0f, modelCenter + worldPos);
    // cube->rotationZ(10.0f, modelCenter + worldPos);
    cube->rotationAxisQuaternion(
        25.0f,
        utils::Vec4::Vector(1.0f, 1.0f, 0.0f),
        worldPos
    );
    this->meshes.push_back(cube);
    
    this->camera->move(0.0f, 1.0f, 2.0f);
    this->camera->rotate(-15.0f, utils::Vec4::Vector(0.0f, 0.0f, 1.0f));
    
    Light *mainLight = new Light(
        -1.0f, 1.4f, -0.2f,
        utils::RGB(0.7f, 0.7f, 0.7f),
        this
    );
    this->lights.push_back(mainLight);
}

// void Scene::setObserverPosition(float x, float y, float z) {
//     this->observerPosition.x = x;
//     this->observerPosition.y = y;
//     this->observerPosition.z = z;
// }

std::vector<SDL_Color> Scene::traceRays() {
    std::cout << "Camera Eye: "; camera->getEye().print();
    std::cout << "Camera Forward: "; camera->getForward().print();
    std::cout << "Camera Up: "; camera->getUp().print();
    std::cout << "Camera Right: "; camera->getRight().print();
    std::cout << "DWindow: " << this->DWindow << std::endl;

    float Dx = this->WIDTH / this->nCol;
    float Dy = this->HEIGHT / this->nRow;
    
    utils::RGB bgColor  = {0.1f, 0.1f, 0.1f};
    
    std::vector<SDL_Color> canvas(this->nRow * this->nCol, bgColor.toSDLColor()); // armazena todos os pixels com a cor de fundo
    
    float Z = -this->DWindow; // Z coordenada da janela
    for (int l = 0; l < this->nRow; l++) {
        float Y = this->HEIGHT/2 - Dy/2 - l*Dy; // Y coordenada do pixel
        for(int c = 0; c < this->nCol; c++) {
            float X = -this->WIDTH/2 + Dx/2 + c*Dx; // X coordenada do pixel
            
            utils::Vec4 forward = camera->getForward();
            utils::Vec4 right = camera->getRight();
            utils::Vec4 up = camera->getUp();
            
            
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

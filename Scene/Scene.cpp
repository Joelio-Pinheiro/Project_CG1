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


    Flat *floor = new Flat(
        utils::Vec4::Point(0.0f, -1.0f, 0.0f),
        utils::Vec4::Vector(0.0f, 1.0f, 0.0f)
    );
    Texture* floorTexture = new Texture("models/madeira.png");
    utils::Material floorMat;

    floorMat.setTexture(floorTexture);
    // floor->setDiffuse(0.1f, 0.6f, 0.1f);
    floorMat.setSpecular(0.0f, 0.0f, 0.0f);
    floorMat.setShininess(2.0f);
    floor->setMaterial(floorMat);
    this->flats.push_back(floor);
    
    Flat *back = new Flat(
        utils::Vec4::Point(0.0f, 0.0f, -10.0f),
        utils::Vec4::Vector(0.0f, 0.0f, 1.0f)
    );
    back->setDiffuse(0.2f, 0.2f, 0.5f);
    back->setSpecular(0.0f, 0.0f, 0.0f);
    back->setShininess(1.0f);
    this->flats.push_back(back);
    
    // Objetos Mesh
    
    Mesh *object = new Mesh();
    bool test = object->loadFromOBJ("models/humanoid_tri.obj");
    
    if (test) {
            std::cout << "OBJ file loaded successfully." << std::endl;
        
            utils::Material redMat;
            redMat.diffuse = {0.8f, 0.2f, 0.2f};
            redMat.specular = {0.7f, 0.3f, 0.3f};
            redMat.shininess = 16.0f;
            object->setMaterial(redMat);
        
            // 🔁 Escalar e transladar o modelo para o campo de visão
            float scale = 0.3f;
            float angleY = M_PI / 2;      // 180° para virar de frente
            float angleX = -M_PI / 2; // -90° para levantar o modelo
        
            for (Triangle *const t : object->getTriangles()) {
                    utils::Vec4 p1 = t->getP1() * scale;
                    utils::Vec4 p2 = t->getP2() * scale;
                    utils::Vec4 p3 = t->getP3() * scale;
            
                    // Rotação em X
                    auto rotateX = [&](const utils::Vec4 &p) -> utils::Vec4 {
                            float y = p.y * cos(angleX) - p.z * sin(angleX);
                            float z = p.y * sin(angleX) + p.z * cos(angleX);
                            return utils::Vec4::Point(p.x, y, z);
                        };
                
                        // Rotação em Y (para o modelo olhar pra câmera)
                        auto rotateY = [&](const utils::Vec4 &p) -> utils::Vec4 {
                                float x = p.x * cos(angleY) + p.z * sin(angleY);
                                float z = -p.x * sin(angleY) + p.z * cos(angleY);
                                return utils::Vec4::Point(x, p.y, z);
                            };
                    
                            p1 = rotateY(rotateX(p1));
                            p2 = rotateY(rotateX(p2));
                            p3 = rotateY(rotateX(p3));
                    
                            utils::Vec4 translation = utils::Vec4::Vector(0, -0.5f, -7.0f);
                            t->setP1(p1 + translation);
                            t->setP2(p2 + translation);
                            t->setP3(p3 + translation);
                        }
                    
                        this->meshes.push_back(object);
                    }
                    
    Mesh *pyramid = new Mesh();
    pyramid->addTriangle({0, 1, 0, 1}, {-1, 0, 1, 1}, {1, 0, 1, 1});
    pyramid->addTriangle({0, 1, 0, 1}, {1, 0, 1, 1}, {1, 0, -1, 1});
    pyramid->addTriangle({0, 1, 0, 1}, {1, 0, -1, 1}, {-1, 0, -1, 1});
    pyramid->addTriangle({0, 1, 0, 1}, {-1, 0, -1, 1}, {-1, 0, 1, 1});
                    
    utils::Material redMat;
    redMat.diffuse = {0.8f, 0.2f, 0.2f};
    redMat.specular = {0.8f, 0.3f, 0.3f};
    redMat.shininess = 16.0f;
    pyramid->setMaterial(redMat);
                    
    // Transladar a pirâmide um pouco pra frente e pra cima do chão
    for (Triangle *const t : pyramid->getTriangles()) {
        t->setP1(t->getP1() + utils::Vec4::Vector(0, -0.5f, -6.0f));
        t->setP2(t->getP2() + utils::Vec4::Vector(0, -0.5f, -6.0f));
        t->setP3(t->getP3() + utils::Vec4::Vector(0, -0.5f, -6.0f));
    }
    this->meshes.push_back(pyramid);
                    
                    
    Mesh *cube = new Mesh();
    cube->addTriangle({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f, -0.5f, 1}, {0.5f, 1.0f, -0.5f, 1});
    cube->addTriangle({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 1.0f, -0.5f, 1}, {-0.5f, 1.0f, -0.5f, 1});
    cube->addTriangle({-0.5f, 0.0f, 0.5f, 1}, {0.5f, 0.0f, 0.5f, 1}, {0.5f, 1.0f, 0.5f, 1});
    cube->addTriangle({-0.5f, 0.0f, 0.5f, 1}, {0.5f, 1.0f, 0.5f, 1}, {-0.5f, 1.0f, 0.5f, 1});
    cube->addTriangle({-0.5f, 1.0f, -0.5f, 1}, {0.5f, 1.0f, -0.5f, 1}, {0.5f, 1.0f, 0.5f, 1});
    cube->addTriangle({-0.5f, 1.0f, -0.5f, 1}, {0.5f, 1.0f, 0.5f, 1}, {-0.5f, 1.0f, 0.5f, 1});
    cube->addTriangle({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f, 0.5f, 1});
    cube->addTriangle({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f, 0.5f, 1}, {-0.5f, 0.0f, 0.5f, 1});
    
    utils::Material blueMat;
    blueMat.diffuse = {0.2f, 0.2f, 0.8f};
    blueMat.specular = {0.5f, 0.5f, 0.8f};
    blueMat.shininess = 12.0f;
    cube->setMaterial(blueMat);
    
    for (Triangle *const triangle : cube->getTriangles()) {
        triangle->setP1(triangle->getP1() + utils::Vec4::Vector(2.5f, -0.5f, -6.5f));
        triangle->setP2(triangle->getP2() + utils::Vec4::Vector(2.5f, -0.5f, -6.5f));
        triangle->setP3(triangle->getP3() + utils::Vec4::Vector(2.5f, -0.5f, -6.5f));
    }
    this->meshes.push_back(cube);
    
    
    Light *mainLight = new Light(
        -5.0f, 8.0f, -3.0f,
        utils::RGB(0.8f, 0.8f, 0.8f),
        this
    );
    this->lights.push_back(mainLight);
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

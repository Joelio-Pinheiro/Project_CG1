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
        
        float scale = 0.3f;
            
        object->scale(scale, scale, scale, utils::Vec4::Point(0,0,0));
        object->rotationX(-90.0f, utils::Vec4::Point(0,0,0));
        object->rotationY(90.0f, utils::Vec4::Point(0,0,0));    
        
        utils::Vec4 translation = utils::Vec4::Vector(0, -0.5f, -7.0f);
        object->translation(translation);
                    
        object->build();
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
        t->translation(utils::Vec4::Vector(0, -0.5f, -6.0f));
    }
    pyramid->build();
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
        triangle->translation(utils::Vec4::Vector(2.5f, -0.5f, -6.5f));
    }
    cube->build();
    this->meshes.push_back(cube);
    
    float rad30 = 20.0f * (M_PI / 180.0f);
    float rad45 = 35.0f * (M_PI / 180.0f);

    Light* spotLight = new Light(
        utils::Vec4::Vector(0.0f, -1.0f, -1.0f), // direção da luz (para frente)
        utils::Vec4::Point(0.0f, 8.0f, -1.0f),     // posição da luz
        rad30,                  // cutoff interno
        rad45,                  // cutoff externo
        utils::RGB(1.0f, 0.95f, 0.85f),           // intensidade
        this
    );

    this->lights.push_back(spotLight);
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
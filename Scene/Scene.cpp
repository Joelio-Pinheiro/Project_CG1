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
#include <omp.h>

Scene::Scene(float width, float height, float DWindow, int nRow, int nCol, utils::window *window){
    this->WIDTH = width;
    this->HEIGHT = height;
    this->window = window;
    this->DWindow = DWindow;
    this->nRow = nRow;
    this->nCol = nCol;

    this->window->setPosition(0, 0, this->DWindow);
    this->setCamera(new Camera(PERSPECTIVE));
    this->setAmbientLight(0.15f, 0.15f, 0.15f); 

    // ---- MATERIAIS ----
    utils::Material skyMat;
    Texture* skyTex = new Texture("models/ceu_noite.png");
    skyMat.setTexture(skyTex);
    skyMat.setDiffuse(0.2f, 0.45f, 0.9f);
    skyMat.setSpecular(0.0f, 0.0f, 0.0f);
    skyMat.setShininess(1.0f);

    utils::Material snowMat;
    Texture* snowTex = new Texture("models/neve.png");
    snowMat.setTexture(snowTex);
    snowMat.setDiffuse(0.95f, 0.95f, 0.95f);
    snowMat.setSpecular(0.2f, 0.2f, 0.2f);
    snowMat.setShininess(8.0f);

    utils::Material trunkMat;
    trunkMat.setDiffuse(0.35f, 0.2f, 0.12f);
    trunkMat.setSpecular(0.04f, 0.03f, 0.02f);
    trunkMat.setShininess(6.0f);

    utils::Material foliageMat;
    foliageMat.setDiffuse(0.06f, 0.45f, 0.14f);
    foliageMat.setSpecular(0.08f, 0.08f, 0.08f);
    foliageMat.setShininess(6.0f);

    utils::Material ornamentMat;
    ornamentMat.setDiffuse(0.9f, 0.1f, 0.15f);
    ornamentMat.setSpecular(0.9f, 0.9f, 0.9f);
    ornamentMat.setShininess(64.0f);

    utils::Material tableMat;
    tableMat.setDiffuse(0.12f, 0.06f, 0.02f);
    tableMat.setSpecular(0.05f, 0.04f, 0.03f);
    tableMat.setShininess(8.0f);

    utils::Material snowmanMat;
    snowmanMat.setDiffuse(0.98f, 0.98f, 0.98f);
    snowmanMat.setSpecular(0.18f, 0.18f, 0.18f);
    snowmanMat.setShininess(20.0f);

    utils::Material eyeMat;
    eyeMat.setDiffuse(0.1f, 0.1f, 0.1f);
    eyeMat.setSpecular(0.1f, 0.1f, 0.1f);
    eyeMat.setShininess(5.0f);

    // ---------- CÉU ----------
    Flat* sky = new Flat(
        utils::Vec4::Point(0.0f, 0.0f, 80.0f),
        utils::Vec4::Vector(0.0f, 0.0f, -1.0f)
    );
    sky->setMaterial(skyMat);
    this->flats.push_back(sky);

    // ---------- CHÃO (NEVE) ----------
    Flat *floor = new Flat(
        utils::Vec4::Point(0.0f, 0.0f, 0.0f),
        utils::Vec4::Vector(0.0f, 1.0f, 0.0f)
    );
    floor->setMaterial(snowMat);
    this->flats.push_back(floor);

    // // ---------- ÁRVORES (tronco + copa) ----------
    {
        std::vector<utils::Vec4> treePos = {
            utils::Vec4::Point(-1.4f, 0.0f, 3.6f), // frente esquerda
        };

        for (int i = 0; i < (int)treePos.size(); ++i) {
            float trunkRadius = 0.13f;
            float trunkHeight = 1.0f;
            Cylinder* trunk = new Cylinder(trunkRadius, trunkHeight,
                treePos[i], utils::Vec4::Vector(0,1,0), true);
            trunk->setMaterial(trunkMat);
            this->cylinders.push_back(trunk);

            float coneR = 1.3f - 0.12f * i;
            float coneH = 3.4f - 0.15f * i;
            utils::Vec4 baseCenter = utils::Vec4::Point(
                treePos[i].x,
                trunkHeight,
                treePos[i].z
            );
            Cone* foliage = new Cone(coneR, coneH, baseCenter, utils::Vec4::Vector(0,1,0));
            foliage->setMaterial(foliageMat);
            this->cones.push_back(foliage);
        }
    }

    // // ---------- BONECO DE NEVE (2 esferas + olhos) ----------
    {
        float bx = -1.5f;
        float bz = 1.0f;

        float r1 = 0.5f;
        float y1 = r1;
        Sphere* s1 = new Sphere(r1, bx, y1, bz);
        s1->setMaterial(snowmanMat);
        this->spheres.push_back(s1);

        float r2 = 0.36f;
        float y2 = y1 + r1 + r2 - 0.02f; 
        Sphere* s2 = new Sphere(r2, bx, y2, bz);
        s2->setMaterial(snowmanMat);
        this->spheres.push_back(s2);


        // olhos
        float eyeOffsetZ = r2 + 0.02f;
        Sphere* eyeL = new Sphere(0.045f, bx - 0.1f, y2 + 0.04f, bz - eyeOffsetZ);
        eyeL->setMaterial(eyeMat);
        Sphere* eyeR = new Sphere(0.045f, bx + 0.1f, y2 + 0.04f, bz - eyeOffsetZ);
        eyeR->setMaterial(eyeMat);
        this->spheres.push_back(eyeL);
        this->spheres.push_back(eyeR);

        float noseRadius = 0.05f;
        float noseHeight = 0.22f;

        // centro da base do cone (ligeiramente fora da esfera)
        utils::Vec4 noseBaseCenter = utils::Vec4::Point(
            bx,
            y2 - 0.05f,
            bz - r2 - 0.01f
        );

        Cone* nose = new Cone(
            noseRadius,
            noseHeight,
            noseBaseCenter,
            utils::Vec4::Vector(0.0f, 0.0f, -1.0f) // aponta para frente (-Z)
        );

        utils::Material noseMat;
        noseMat.setDiffuse(0.95f, 0.5f, 0.1f);   // laranja
        noseMat.setSpecular(0.3f, 0.3f, 0.3f);
        noseMat.setShininess(8.0f);

        nose->setMaterial(noseMat);
        this->cones.push_back(nose);
    }

    // ---------- MESA (MALHA) ----------
    auto addBoxToMesh = [&](Mesh* m, utils::Vec4 center, float sx, float sy, float sz){
        utils::Vec4 v[8];
        float hx = sx * 0.5f, hy = sy * 0.5f, hz = sz * 0.5f;
        v[0] = utils::Vec4::Point(center.x - hx, center.y - hy, center.z - hz);
        v[1] = utils::Vec4::Point(center.x + hx, center.y - hy, center.z - hz);
        v[2] = utils::Vec4::Point(center.x + hx, center.y + hy, center.z - hz);
        v[3] = utils::Vec4::Point(center.x - hx, center.y + hy, center.z - hz);
        v[4] = utils::Vec4::Point(center.x - hx, center.y - hy, center.z + hz);
        v[5] = utils::Vec4::Point(center.x + hx, center.y - hy, center.z + hz);
        v[6] = utils::Vec4::Point(center.x + hx, center.y + hy, center.z + hz);
        v[7] = utils::Vec4::Point(center.x - hx, center.y + hy, center.z + hz);

        // frente (+z)
        m->addTriangle(v[4], v[5], v[6]);
        m->addTriangle(v[4], v[6], v[7]);
        // trás (-z)
        m->addTriangle(v[1], v[0], v[3]);
        m->addTriangle(v[1], v[3], v[2]);
        // topo (+y)
        m->addTriangle(v[3], v[7], v[6]);
        m->addTriangle(v[3], v[6], v[2]);
        // base (-y)
        m->addTriangle(v[0], v[1], v[5]);
        m->addTriangle(v[0], v[5], v[4]);
        // esquerda (-x)
        m->addTriangle(v[0], v[4], v[7]);
        m->addTriangle(v[0], v[7], v[3]);
        // direita (+x)
        m->addTriangle(v[5], v[1], v[2]);
        m->addTriangle(v[5], v[2], v[6]);
    };

    // table parameters: top center and sizes
    utils::Vec4 tableCenter = utils::Vec4::Point(0.0f, 0.65f, 0.8f); 
    float top_sx = 1.2f, top_sy = 0.12f, top_sz = 0.8f;
    Mesh* tableMesh = new Mesh();
    addBoxToMesh(tableMesh, tableCenter, top_sx, top_sy, top_sz);

    // legs
    float legHalfOffsetX = top_sx * 0.5f - 0.12f; // inset a little
    float legHalfOffsetZ = top_sz * 0.5f - 0.12f;
    float legHeight = 0.6f;
    float legThickness = 0.12f;
    utils::Vec4 legCenters[4] = {
        utils::Vec4::Point(-legHalfOffsetX, tableCenter.y - (top_sy/2.0f) - (legHeight/2.0f), tableCenter.z - legHalfOffsetZ),
        utils::Vec4::Point( legHalfOffsetX, tableCenter.y - (top_sy/2.0f) - (legHeight/2.0f), tableCenter.z - legHalfOffsetZ),
        utils::Vec4::Point(-legHalfOffsetX, tableCenter.y - (top_sy/2.0f) - (legHeight/2.0f), tableCenter.z + legHalfOffsetZ),
        utils::Vec4::Point( legHalfOffsetX, tableCenter.y - (top_sy/2.0f) - (legHeight/2.0f), tableCenter.z + legHalfOffsetZ)
    };
    for (int i = 0; i < 4; ++i) {
        addBoxToMesh(tableMesh, legCenters[i], legThickness, legHeight, legThickness);
    }
    tableMesh->setMaterial(tableMat);
    tableMesh->build();
    this->meshes.push_back(tableMesh);

    // ---------- ENFEITES (presentes sobre a mesa) ----------
    {
        float baseX = tableCenter.x - 0.45f;
        float step  = 0.45f;
        float baseY = tableCenter.y + (top_sy / 2.0f);
        float z     = tableCenter.z;

        std::vector<float> scales = {0.15f, 0.22f, 0.30f};
        std::vector<utils::Vec4> colors = {
            utils::Vec4::Point(0.9f, 0.1f, 0.15f), // vermelho
            utils::Vec4::Point(0.95f, 0.8f, 0.2f), // amarelo
            utils::Vec4::Point(0.1f, 0.6f, 0.95f) // azul
        };

        auto addCubeGeometry = [&](Mesh* cube){
            auto add = [&](const utils::Vec4 &a, const utils::Vec4 &b, const utils::Vec4 &c){
                cube->addTriangle(a,c,b);
            };

            // face -Z
            add({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f, -0.5f, 1}, {0.5f, 1.0f, -0.5f, 1});
            add({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 1.0f, -0.5f, 1}, {-0.5f, 1.0f, -0.5f, 1});
            // face +Z
            add({-0.5f, 0.0f,  0.5f, 1}, {0.5f, 0.0f,  0.5f, 1}, {0.5f, 1.0f,  0.5f, 1});
            add({-0.5f, 0.0f,  0.5f, 1}, {0.5f, 1.0f,  0.5f, 1}, {-0.5f, 1.0f,  0.5f, 1});
            // topo +Y
            add({-0.5f, 1.0f, -0.5f, 1}, {0.5f, 1.0f, -0.5f, 1}, {0.5f, 1.0f,  0.5f, 1});
            add({-0.5f, 1.0f, -0.5f, 1}, {0.5f, 1.0f,  0.5f, 1}, {-0.5f, 1.0f,  0.5f, 1});
            // base -Y
            add({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f,  0.5f, 1});
            add({-0.5f, 0.0f, -0.5f, 1}, {0.5f, 0.0f,  0.5f, 1}, {-0.5f, 0.0f,  0.5f, 1});
        };

        for (int i = 0; i < 3; ++i) {

            Mesh* cube = new Mesh();

            addCubeGeometry(cube);

            // ---------- MATERIAL ----------
            utils::Material m = ornamentMat;
            m.setDiffuse(colors[i].x, colors[i].y, colors[i].z);

            if (i == 2) {
                m.setSpecular(0.9f, 0.9f, 0.9f);
                m.setShininess(64.0f);
            }

            cube->setMaterial(m);

            // ---------- TRANSFORMAÇÕES ----------
            float s = scales[i];
            float cubeHeight = s * 1.0f;

            cube->scale(s, s, s, utils::Vec4::Point(0.0f, 0.5f, 0.0f));

            if (i == 1) {
                cube->rotationY(30.0f, utils::Vec4::Point(0.0f, 0.5f, 0.0f));
            }
            if (i == 2) {
                cube->shear(0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, utils::Vec4::Point(0.0f, 0.5f, 0.0f));
            }
            float ty = baseY - 0.5f * (1.0f - s) + 0.001f;

            utils::Vec4 t(
                baseX + i * step,
                ty,
                z,
                0
            );

            cube->translation(t);

            cube->build();
            this->meshes.push_back(cube);
        }
    }

    // ---------- LUZES ----------
    Light* pointLight = new Light(
        1.8f ,5.0f, -3.0f,
        utils::RGB(0.5f, 0.75f, 0.6f),
        this
    );
    // this->lights.push_back(pointLight);

    // Spot focando a mesa (melhora leitura do foco)
    float inner = 12.0f * (M_PI / 180.0f);
    float outer = 30.0f * (M_PI / 180.0f);
    Light* spot = new Light(
        utils::Vec4::Vector(0.0f, -1.0f, 0.0f),             // direção (apontando diretamente para baixo)
        utils::Vec4::Point(tableCenter.x, tableCenter.y + 2.2f, tableCenter.z), // posição do spot acima da mesa
        inner,
        outer,
        utils::RGB(1.0f, 1.0f, 0.96f),
        this
    );
    this->lights.push_back(spot);

    Light* dir = new Light(
        utils::Vec4::Vector(-0.4f, -1.0f, 0.2f),
        utils::RGB(0.45f, 0.5f, 0.6f),
        this
    );
    this->lights.push_back(dir);

    // ---------- CÂMERA ----------
    // camera->setEye(utils::Vec4::Point(0.0f, 1.2f, -1.6f));
    // camera->setForward(utils::Vec4::Point(0.0f, 0.7f, 1.8f));
    // camera->setUp(utils::Vec4::Vector(0.0f, 1.0f, 0.0f));

    // 1 ponto de fuga
    camera->setEye(utils::Vec4::Point(0.0f, 1.2f, -4.0f));
    camera->setForward(utils::Vec4::Point(0.0f, 1.2f, 2.0f));
    camera->setUp(utils::Vec4::Vector(0.0f, 1.0f, 0.0f));


    // 2 pontos de fuga
    // camera->setEye(utils::Vec4::Point(0, 2, -5));
    // camera->setForward(utils::Vec4::Vector(1, 0, 1).normalize());
    // camera->setUp(utils::Vec4::Vector(0, 1, 0));

    // 3 pontos de fuga
    // camera->setEye(utils::Vec4::Point(0, 2, -5));
    // camera->setForward(utils::Vec4::Vector(1, -0.5f, 1).normalize());
    // camera->setUp(utils::Vec4::Vector(0, 1, 0));

}


std::vector<SDL_Color> Scene::traceRays() {

    float Dx = this->WIDTH / this->nCol;
    float Dy = this->HEIGHT / this->nRow;
    
    utils::RGB bgColor  = {0.1f, 0.1f, 0.1f};
    
    std::vector<SDL_Color> canvas(this->nRow * this->nCol, bgColor.toSDLColor()); // armazena todos os pixels com a cor de fundo
    
    float Z = this->DWindow; // Z coordenada da janela
    #pragma omp parallel for collapse(2) schedule(dynamic, 8)
    for (int l = 0; l < this->nRow; l++) {
        for(int c = 0; c < this->nCol; c++) {
            float Y = this->HEIGHT/2 - Dy/2 - l*Dy; // Y coordenada do pixel
            float X = -this->WIDTH/2 + Dx/2 + c*Dx; // X coordenada do pixel
            
            if (c % this->speedRender != 0 || l % this->speedRender != 0){
                canvas[l * this->nCol + c] = utils::RGB(0.0f, 0.0f, 0.0f).toSDLColor();
                continue;
            }
            
            utils::Vec4 pixel =
                camera->getEye()
                + camera->getForward() * Z     // Z positivo
                + camera->getRight()   * X
                + camera->getUp()      * Y;

            Ray ray(utils::Vec4::Point(0,0,0), utils::Vec4::Vector(0,0,0)); // raio default

            if (camera->getProjection() == PERSPECTIVE) {
                utils::Vec4 dir = (pixel - camera->getEye()).normalize();
                ray = Ray(camera->getEye(), dir);
            } else if (camera->getProjection() == ORTHOGRAPHIC) {
                utils::Vec4 origin =
                    camera->getEye()
                    + camera->getRight() * X
                    + camera->getUp() * Y;

                utils::Vec4 dir = camera->getForward().normalize();
                ray = Ray(origin, dir);
            } else if (camera->getProjection() == OBLIQUE) { // OBLIQUE
                utils::Vec4 origin =
                        camera->getEye()
                        + camera->getRight() * X
                        + camera->getUp() * Y;

                utils::Vec4 dir = (
                    camera->getForward()
                    + camera->getRight() * 0.5f
                    + camera->getUp() * 0.5f
                ).normalize();

                ray = Ray(origin, dir);
            }
            
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
    ensureTexture(this->nCol, this->nRow);

    void* pixels = nullptr;
    int pitch = 0;

    if (SDL_LockTexture(canvasTexture, nullptr, &pixels, &pitch) != 0) {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError() << std::endl;
        return;
    }

    // pitch é bytes por linha; cada pixel é 4 bytes (RGBA8888)
    for (int y = 0; y < nRow; ++y) {
        Uint8* rowBytes = (Uint8*)pixels + y * pitch;
        Uint32* row = (Uint32*)rowBytes;
        for (int x = 0; x < nCol; ++x) {
            int i = y * nCol + x;
            SDL_Color c = canvas[i];
            // use canvasFormat alocado uma vez em ensureTexture
            row[x] = SDL_MapRGBA(canvasFormat, c.r, c.g, c.b, c.a);
        }
    }

    SDL_UnlockTexture(canvasTexture);
}

void Scene::ensureTexture(int w, int h) { // garante que a textura do canvas tenha o tamanho correto
    SDL_Renderer* renderer = window->getRenderer();
    if (!canvasTexture || texW != w || texH != h) {
        if (canvasTexture) {
            SDL_DestroyTexture(canvasTexture);
            SDL_FreeFormat(canvasFormat);
        }

        canvasTexture = SDL_CreateTexture(renderer,
                                          SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          w, h);
        canvasFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
        texW = w; texH = h;

        SDL_SetTextureBlendMode(canvasTexture, SDL_BLENDMODE_NONE);
    }
}

void Scene::render() {
    std::vector<SDL_Color> canvas = this->traceRays();
    this->drawCanvas(canvas);
}

// Interatividade
void Scene::pick(int mx, int my)
{
    float Dx = WIDTH  / nCol;
    float Dy = HEIGHT / nRow;

    float X = -WIDTH  / 2.0f + Dx / 2.0f + mx * Dx;
    float Y =  HEIGHT / 2.0f - Dy / 2.0f - my * Dy;
    float Z = DWindow;

    utils::Vec4 pixel =
        camera->getEye()
        + camera->getForward() * Z
        + camera->getRight()   * X
        + camera->getUp()      * Y;

    utils::Vec4 dir = (pixel - camera->getEye()).normalize();
    Ray ray(camera->getEye(), dir);

    utils::HitInfo closestHit;
    closestHit.hit = false;
    closestHit.t   = std::numeric_limits<float>::max();

    utils::Material hitMaterial;

    auto tryHit = [&](const utils::HitInfo& hit, const utils::Material& m)
    {
        if (hit.hit && hit.t < closestHit.t)
        {
            closestHit   = hit;
            hitMaterial  = m;
        }
    };

    for (Sphere*   s : spheres)   tryHit(s->intersects(ray), s->getMaterial());
    for (Cylinder* c : cylinders) tryHit(c->intersects(ray), c->getMaterial());
    for (Cone*     c : cones)     tryHit(c->intersects(ray), c->getMaterial());
    for (Triangle* t : triangles) tryHit(t->intersects(ray), t->getMaterial());
    for (Mesh*     m : meshes)    tryHit(m->intersects(ray), m->getMaterial());
    for (Flat*     f : flats)     tryHit(f->intersects(ray), f->getMaterial());

    if (!closestHit.hit)
    {
        std::cout << "[PICK] Nothing hit\n";
        return;
    }

    utils::RGB total(0, 0, 0);

    for (Light* L : lights)
        total = total + L->ComputeLighting(closestHit, hitMaterial, dir);

    std::cout << "\n[PICK HIT]\n";
    std::cout << "Type: " << closestHit.type << "\n";
    std::cout << "t = " << closestHit.t << "\n";
    std::cout << "Point: "; closestHit.point.print();
    std::cout << "Normal: "; closestHit.normal.print();
    std::cout << "Color: R=" << total.r
              << " G=" << total.g
              << " B=" << total.b << "\n";
}

void Scene::handleEvent(const SDL_Event& e){
   switch (e.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_RIGHT){
                mouseCaptured = true;
                SDL_SetRelativeMouseMode(SDL_TRUE);
                ignoreNextMouseMotion = true;
            }
            if(e.button.button == SDL_BUTTON_LEFT){
                pick(e.button.x, e.button.y);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_RIGHT){
                mouseCaptured = false;
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            break;

        case SDL_MOUSEMOTION:
            if (mouseCaptured) {
                if (ignoreNextMouseMotion) {
                    // consome e descarta este evento
                    ignoreNextMouseMotion = false;
                    break;
                }

                float dx = (float)e.motion.xrel * mouseSensitivity;
                float dy = (float)e.motion.yrel * mouseSensitivity;

                camera->yaw(-dx);
                camera->pitchRotate(-dy);
                this->markDirty();
            }
            break;


        case SDL_MOUSEWHEEL:
            if(e.wheel.y > 0) {
                DWindow += zoomSpeed; // zoom in
            } else {
                DWindow -= zoomSpeed;   // zoom out
                if (DWindow < 0.1f) DWindow = 0.1f;
            }
            this->markDirty();
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_W: camera->move(0,0,moveSpeed*2.0f); break;
                case SDL_SCANCODE_S: camera->move(0,0,-moveSpeed*2.0f); break;
                case SDL_SCANCODE_A: camera->move(-moveSpeed*2.0f,0,0); break;
                case SDL_SCANCODE_D: camera->move(moveSpeed*2.0f,0,0); break;
                case SDL_SCANCODE_Q: camera->move(0,-moveSpeed*2.0f,0); break;
                case SDL_SCANCODE_E: camera->move(0,moveSpeed*2.0f,0); break;
            }
            this->markDirty();
            break;

    }
}

void Scene::handleKeyboard(const Uint8* keys)
{
    float speed = this->moveSpeed;
    if (keys[SDL_SCANCODE_W])
        camera->move(0, 0, speed);
        this->markDirty();

    if (keys[SDL_SCANCODE_S])
        camera->move(0, 0, -speed);
        this->markDirty();

    if (keys[SDL_SCANCODE_A])
        camera->move(-speed, 0, 0);
        this->markDirty();

    if (keys[SDL_SCANCODE_D])
        camera->move(speed, 0, 0);
        this->markDirty();

    if (keys[SDL_SCANCODE_Q])
        camera->move(0, -speed, 0);
        this->markDirty();

    if (keys[SDL_SCANCODE_E])
        camera->move(0, speed, 0);
        this->markDirty();
}

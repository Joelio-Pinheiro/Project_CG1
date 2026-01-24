#include "../headers/Light.h"
#include "../headers/utils.h"
#include <SDL.h>
#include <vector>
#include <algorithm>

Light::Light(float x, float y, float z, utils::RGB intensity, Scene* scene) { // Luz Pontual
    this->positionF = utils::Vec4::Point(x, y, z);
    this->intensityF = intensity;
    this->scene = scene;
    this->ambientLight = this->scene->getAmbientLight();

    this->type = LightType::POINT;
}
Light::Light(utils::Vec4 direction, utils::RGB intensity, Scene* scene) { // Luz Direcional
    this->directionF = direction.normalize();
    this->intensityF = intensity;
    this->scene = scene;
    this->ambientLight = this->scene->getAmbientLight();

    this->type = LightType::DIRECTIONAL;
}
Light::Light(utils::Vec4 direction, utils::Vec4 position, float cutoff, float outerCutoff, utils::RGB intensity, Scene* scene) { // Luz Spot
    this->directionF = direction.normalize();
    this->positionF = position;
    this->cutoff = cutoff;
    this->outerCutoff = outerCutoff;
    this->intensityF = intensity;
    this->scene = scene;
    this->ambientLight = this->scene->getAmbientLight();

    this->type = LightType::SPOT;
}


utils::RGB Light::ComputeLighting(const utils::HitInfo& hitInfo, const utils::Material& material, const utils::Vec4& RayDirection) {
    utils::Vec4 normal =  hitInfo.normal; // N = (Pi - C) / |Pi - C|
    utils::Vec4 lightDir = getLightDir(hitInfo); // L
    utils::Vec4 viewDir  = (RayDirection * -1).normalize(); // V = -dr
    utils::Vec4 reflectDir = ((normal * (2.0f * normal.dot(lightDir))) - lightDir).normalize(); // R = 2(N . L)N - L

    utils::RGB colorDifuse = material.getDiffuse();
    utils::RGB colorSpecular = material.getSpecular();
    float shininess = material.getShininess();

    if (material.hasTexture()) { // Amostragem de textura
        colorDifuse = material.texture->sample(hitInfo.u, hitInfo.v);
        // colorDifuse = colorDifuse.AtSign(textureColor);
    }


    // Componente ambiente
    utils::RGB ambientF = this->ambientLight.AtSign(colorDifuse); // Iamb @ Eamb

    float spotFactor = 1.0f;
    if (type == LightType::SPOT) {
        utils::Vec4 L = lightDir;
        utils::Vec4 D = directionF.normalize();
        float theta = L.dot(D * -1); // L . D

        float inner = std::cos(cutoff);
        float outer = std::cos(outerCutoff);

        if (theta < outer) {
            return ambientF; // Totalmente fora
        }
        if(theta < inner) { // parcialmente dentro
            float epsilon = inner - outer;
            spotFactor = (theta - outer) / epsilon;

            spotFactor = std::clamp(spotFactor, 0.0f, 1.0f); // integra na difussa e especular
        }
    }
    

    utils::RGB finalColor = ambientF;
    bool shadowColor = this->Shadow(hitInfo, lightDir);
    if (shadowColor) {
        return finalColor;
    }

    // Componente difusa
    float ndotl = std::max(normal.dot(lightDir), 0.0f);
    utils::RGB diffuseF = this->intensityF.AtSign(colorDifuse) * ndotl * spotFactor; // IF @ Edif * (N . L) . spotFactor

    // Componente especular
    float spec = std::pow(std::max(reflectDir.dot(viewDir), 0.0f), shininess); // spec = (R . V)^n
    utils::RGB specularF = this->intensityF.AtSign(colorSpecular) * spec * spotFactor; // IF @ Espec * spec . spotFactor

    finalColor = finalColor + diffuseF + specularF;
    finalColor = utils::RGB(
        utils::RGB::clamp(finalColor.r, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.g, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.b, 0.0f, 1.0f)
    );
    return finalColor;
}

bool Light::Shadow (const utils::HitInfo& hitInfo, const utils::Vec4& lightDir) {
    Ray shadowRay(
        hitInfo.point + hitInfo.normal * 0.001f, // Evita acne de sombra
        lightDir
    );


    for (const Sphere* sphere : this->scene->getSpheres()) {
        utils::HitInfo shadowHit = sphere->intersects(shadowRay);

        if (shadowHit.hit && shadowHit.t > 0.001f && shadowHit.t < this->maxDist) {
            return true;
        }

    }
    for (const Cylinder* cylinder : this->scene->getCylinders()) {
        utils::HitInfo shadowHit = cylinder->intersects(shadowRay);

        if (shadowHit.hit && shadowHit.t > 0.001f && shadowHit.t < this->maxDist) {
            return true;
        }
    }
    for (const Cone* cone : this->scene->getCones()) {
        utils::HitInfo shadowHit = cone->intersects(shadowRay);

        if (shadowHit.hit && shadowHit.t > 0.001f && shadowHit.t < this->maxDist) {
            return true;
        }
    }
    for(const Triangle* triangle : this->scene->getTriangles()) {
        utils::HitInfo shadowHit = triangle->intersects(shadowRay);

        if (shadowHit.hit && shadowHit.t > 0.001f && shadowHit.t < this->maxDist) {
            return true;
        }
    }
    for (const Mesh* mesh : this->scene->getMeshes()) {
        utils::HitInfo shadowHit = mesh->intersects(shadowRay);

       if (shadowHit.hit && shadowHit.t > 0.001f && shadowHit.t < this->maxDist) {
            return true;
        }
    }
    
    for (const Flat* flat : this->scene->getFlats()) {
        utils::HitInfo shadowHit = flat->intersects(shadowRay);

        if (shadowHit.hit && shadowHit.t > 0.001f && shadowHit.t < this->maxDist) {
            return true;
        }
    }
    return false; 
}

utils::Vec4 Light::getLightDir(utils::HitInfo hitInfo) {
   utils::Vec4 lightDir;

    if (type == LightType::POINT || type == LightType::SPOT) {
        lightDir = (positionF - hitInfo.point).normalize(); // L = (PF - Pi) / |PF - Pi|
        this->maxDist  = (positionF - hitInfo.point).length();
    } else {
        lightDir = (directionF * -1).normalize(); // L = -D
        this->maxDist  = std::numeric_limits<float>::infinity();
    }

    return lightDir;
}
#include "../headers/Light.h"
#include "../headers/utils.h"
#include <SDL.h>
#include <vector>

Light::Light(float x, float y, float z, utils::RGB intensity, utils::RGB ambient) {
    this->positionF = utils::Vec4::Point(x, y, z);
    this->intensityF = intensity;
    this->ambientLight = ambient;
}



utils::RGB Light::ComputeLighting(const HitInfo& hitInfo, const Sphere* sphere, const std::vector<Sphere*>* spheres, const std::vector<Flat*>* flats, const utils::Vec4& RayDirection) const {
    utils::Vec4 normal =  hitInfo.normal; // N = (Pi - C) / |Pi - C|
    utils::Vec4 lightDir = (this->positionF - hitInfo.point).normalize(); // L = (PF - Pi) / |PF - Pi|
    utils::Vec4 viewDir  = RayDirection * -1; // V = -dr
    utils::Vec4 reflectDir = (normal * (2.0f * normal.dot(lightDir))) - lightDir; // R = 2(N . L)N - L

    utils::RGB colorSphere = sphere->getDiffuse();

    // Componente ambiente
    utils::RGB ambientF = this->ambientLight.AtSign(colorSphere); // Iamb @ Eamb

    utils::RGB finalColor = ambientF;
    bool shadowColor = this->Shadow(hitInfo, spheres, flats, lightDir);
    if (shadowColor) {
        return finalColor;
    }

    // Componente difusa
    float ndotl = std::max(normal.dot(lightDir), 0.0f);
    utils::RGB diffuseF = this->intensityF.AtSign(colorSphere) * ndotl; // IF @ Edif * (N . L)

    // Componente especular
    float shininess = sphere->getShininess();
    float spec = std::pow(std::max(reflectDir.dot(viewDir), 0.0f), shininess); // spec = (R . V)^n
    utils::RGB specularF = this->intensityF.AtSign(sphere->getSpecular()) * spec; // IF @ Espec * spec


    finalColor = finalColor + diffuseF + specularF;
    finalColor = utils::RGB(
        utils::RGB::clamp(finalColor.r, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.g, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.b, 0.0f, 1.0f)
    );

    return finalColor;
}
utils::RGB Light::ComputeLighting(const HitInfo& hitInfo, const Flat* flat, const std::vector<Sphere*>* spheres, const std::vector<Flat*>* flats, const utils::Vec4& RayDirection) const {
    utils::Vec4 normal =  hitInfo.normal; // N = (Pi - C) / |Pi - C|
    utils::Vec4 lightDir = (this->positionF - hitInfo.point).normalize(); // L = (PF - Pi) / |PF - Pi|
    utils::Vec4 viewDir  = RayDirection * -1; // V = -dr
    utils::Vec4 reflectDir = (normal * (2.0f * normal.dot(lightDir))) - lightDir; // R = 2(N . L)N - L

    utils::RGB colorSphere = flat->getDiffuse();

    // Componente ambiente
    utils::RGB ambientF = this->ambientLight.AtSign(colorSphere); // Iamb @ Eamb

    utils::RGB finalColor = ambientF;
    bool shadowColor = this->Shadow(hitInfo, spheres, flats, lightDir);
    if (shadowColor) {
        return finalColor;
    }

    // Componente difusa
    float ndotl = std::max(normal.dot(lightDir), 0.0f);
    utils::RGB diffuseF = this->intensityF.AtSign(colorSphere) * ndotl; // IF @ Edif * (N . L)

    // Componente especular
    float shininess = flat->getShininess();
    float spec = std::pow(std::max(reflectDir.dot(viewDir), 0.0f), shininess); // spec = (R . V)^n
    utils::RGB specularF = this->intensityF.AtSign(flat->getSpecular()) * spec; // IF @ Espec * spec


    finalColor = finalColor + diffuseF + specularF;
    finalColor = utils::RGB(
        utils::RGB::clamp(finalColor.r, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.g, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.b, 0.0f, 1.0f)
    );

    return finalColor;
}

bool Light::Shadow (const HitInfo& hitInfo, const std::vector<Sphere*>* spheres, const std::vector<Flat*>* flats, const utils::Vec4& lightDir) const {
    Ray shadowRay(hitInfo.point, lightDir); // pequeno deslocamento para evitar auto-sombra, alterar no futuro

    for (const Sphere* sphere : *spheres) {
        HitInfo shadowHit = sphere->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    for (const Flat* flat : *flats) {
        HitInfo shadowHit = flat->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    return false; 
}
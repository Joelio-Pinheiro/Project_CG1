#include "../headers/Light.h"
#include "../headers/utils.h"
#include <SDL.h>

Light::Light(float x, float y, float z, utils::RGB intensity, utils::RGB ambient) {
    this->positionF = utils::Vec4::Point(x, y, z);
    this->intensityF = intensity;
    this->ambientLight = ambient;
}



utils::RGB Light::ComputeLighting(const HitInfo& hitInfo, const Sphere* sphere, const std::vector<Sphere*>* spheres, const utils::Vec4& RayDirection) const {
    utils::Vec4 normal =  hitInfo.normal; // N = (Pi - C) / |Pi - C|
    utils::Vec4 lightDir = (this->positionF - hitInfo.point).normalize(); // L = (PF - Pi) / |PF - Pi|
    utils::Vec4 viewDir  = RayDirection * -1; // V = -dr
    utils::Vec4 reflectDir = (normal * (2.0f * normal.dot(lightDir))) - lightDir; // R = 2(N . L)N - L

    utils::RGB colorSphere = sphere->getDiffuse();

    // Componente ambiente
    utils::RGB ambientF = this->ambientLight.AtSign(colorSphere); // Iamb @ Eamb

    utils::RGB finalColor = ambientF;
    bool shadowColor = this->Shadow(hitInfo, spheres, lightDir);
    if (shadowColor) {
        return finalColor;
    }

    // Componente difusa
    utils::RGB diffuseF = this->intensityF.AtSign(colorSphere) * normal.dot(lightDir); // IF @ Edif * (N . L)

    // Componente especular
    float shininess = 10.0f;
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
bool Light::Shadow (const HitInfo& hitInfo, const std::vector<Sphere*>* spheres, const utils::Vec4& lightDir) const {
    Ray shadowRay(hitInfo.point + lightDir * 0.01f, lightDir); // pequeno deslocamento para evitar auto-sombra, alterar no futuro

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
    return false; 
}
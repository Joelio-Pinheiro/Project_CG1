#include "../headers/Light.h"
#include "../headers/utils.h"
#include <SDL.h>
#include <vector>

Light::Light(float x, float y, float z, utils::RGB intensity, Scene* scene) {
    this->positionF = utils::Vec4::Point(x, y, z);
    this->intensityF = intensity;
    this->scene = scene;
    this->ambientLight = this->scene->getAmbientLight();
}



utils::RGB Light::ComputeLighting(const utils::HitInfo& hitInfo, const utils::Material& material, const utils::Vec4& RayDirection) {
    utils::Vec4 normal =  hitInfo.normal; // N = (Pi - C) / |Pi - C|
    utils::Vec4 lightDir = (this->positionF - hitInfo.point).normalize(); // L = (PF - Pi) / |PF - Pi|
    utils::Vec4 viewDir  = (RayDirection * -1).normalize(); // V = -dr
    utils::Vec4 reflectDir = ((normal * (2.0f * normal.dot(lightDir))) - lightDir).normalize(); // R = 2(N . L)N - L

    utils::RGB colorDifuse = material.getDiffuse();
    utils::RGB colorSpecular = material.getSpecular();
    float shininess = material.getShininess();

    if (material.hasTexture()) {
        colorDifuse = material.texture->sample(hitInfo.u, hitInfo.v);
        // colorDifuse = colorDifuse.AtSign(textureColor);
    }


    // Componente ambiente
    utils::RGB ambientF = this->ambientLight.AtSign(colorDifuse); // Iamb @ Eamb

    utils::RGB finalColor = ambientF;
    bool shadowColor = this->Shadow(hitInfo, lightDir);
    if (shadowColor) {
        return finalColor;
    }

    // Componente difusa
    float ndotl = std::max(normal.dot(lightDir), 0.0f);
    utils::RGB diffuseF = this->intensityF.AtSign(colorDifuse) * ndotl; // IF @ Edif * (N . L)

    // Componente especular
    float spec = std::pow(std::max(reflectDir.dot(viewDir), 0.0f), shininess); // spec = (R . V)^n
    utils::RGB specularF = this->intensityF.AtSign(colorSpecular) * spec; // IF @ Espec * spec

    finalColor = finalColor + diffuseF + specularF;
    finalColor = utils::RGB(
        utils::RGB::clamp(finalColor.r, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.g, 0.0f, 1.0f),
        utils::RGB::clamp(finalColor.b, 0.0f, 1.0f)
    );
    return finalColor;
}

bool Light::Shadow (const utils::HitInfo& hitInfo, const utils::Vec4& lightDir) {
    Ray shadowRay(hitInfo.point, lightDir);


    for (const Sphere* sphere : this->scene->getSpheres()) {
        utils::HitInfo shadowHit = sphere->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    for (const Cylinder* cylinder : this->scene->getCylinders()) {
        utils::HitInfo shadowHit = cylinder->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    for (const Cone* cone : this->scene->getCones()) {
        utils::HitInfo shadowHit = cone->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    for(const Triangle* triangle : this->scene->getTriangles()) {
        utils::HitInfo shadowHit = triangle->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    for (const Mesh* mesh : this->scene->getMeshes()) {
        utils::HitInfo shadowHit = mesh->intersects(shadowRay);

        float lightDist = (positionF - hitInfo.point).length();
        if (shadowHit.hit) {
            float hitDist = (shadowHit.point - hitInfo.point).length();
            if (hitDist < lightDist) {
                return true;
            }
        }

    }
    
    for (const Flat* flat : this->scene->getFlats()) {
        utils::HitInfo shadowHit = flat->intersects(shadowRay);

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
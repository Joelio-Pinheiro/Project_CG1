#include "../headers/Light.h"
#include "../headers/utils.h"
#include <SDL.h>

Light::Light(float x, float y, float z, utils::Vec4 intensity) {
    this->positionF = utils::Vec4::Point(x, y, z);
    this->intensityF = intensity;
}



utils::Vec4 Light::ComputeLighting(const HitInfo& hitInfo, const Sphere* sphere, const utils::Vec4& RayDirection) const {
    utils::Vec4 normal =  hitInfo.normal; // N = (Pi - C) / |Pi - C|
    utils::Vec4 lightDir = (this->positionF.operator-(hitInfo.point)).normalize(); // L = (PF - Pi) / |PF - Pi|
    utils::Vec4 viewDir  = RayDirection.operator*(-1); // V = -dr
    utils::Vec4 reflectDir = (normal.operator*(2.0f * normal.dot(lightDir))).operator-(lightDir); // R = 2(N . L)N - L

    // Componente difusa
    utils::Vec4 colorSphere = sphere->getColorsVec4();
    colorSphere = colorSphere / 255.0f; // normaliza para [0,1]
    utils::Vec4 diffuseC = this->intensityF.AtSign(colorSphere).operator*(normal.dot(lightDir)); // IF @ Edif * (N . L)
    
    // Componente especular
    float shininess = 10.0f;
    float spec = std::pow(std::max(reflectDir.dot(viewDir), 0.0f), shininess);
    utils::Vec4 specularC = this->intensityF.AtSign(sphere->getSpecular()).operator*(spec); // IF @ Espec * (R . V)^n

    // Componente ambiente
    utils::Vec4 iAmb = utils::Vec4::Vector(0.1f, 0.1f, 0.1f);
    utils::Vec4 ambientC = iAmb.AtSign(colorSphere); // Iamb @ Eamb

    utils::Vec4 finalColor = ambientC + diffuseC + specularC;
    finalColor = utils::Vec4::Vector(
        utils::Vec4::clamp(finalColor.x, 0.0f, 1.0f),
        utils::Vec4::clamp(finalColor.y, 0.0f, 1.0f),
        utils::Vec4::clamp(finalColor.z, 0.0f, 1.0f)
    );

    return finalColor;
}
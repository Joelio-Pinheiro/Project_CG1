#ifndef LIGHT_H
#define LIGHT_H
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Cylinder.h"
#include "../headers/Cone.h"
#include "../headers/Triangle.h"
#include "../headers/Mesh.h"
#include "../headers/Flat.h"
#include "../headers/Ray.h"
#include "../headers/Scene.h"
#include <SDL.h>
#include <vector>


class Scene;

enum class LightType {
    POINT,
    DIRECTIONAL,
    SPOT
};
class Light {
    private:
        Scene* scene;

        utils::Vec4 positionF; // POINT e SPOT
        utils::Vec4 directionF; // DIRECTIONAL e SPOT
        float cutoff; // SPOT angulo interno graus ou cos
        float outerCutoff; // SPOT angulo externo graus

        utils::RGB intensityF;
        utils::RGB ambientLight;
        LightType type;
        float maxDist;
    public:
        Light(float x, float y, float z, utils::RGB intensity, Scene* scene); // Luz Pontual
        Light(utils::Vec4 direction, utils::RGB intensity, Scene* scene); // Luz Direcional
        Light(utils::Vec4 direction, utils::Vec4 position, float cutoff, float outerCutoff, utils::RGB intensity, Scene* scene); // Luz Spot

        utils::RGB ComputeLighting(const utils::HitInfo& hitInfo, const utils::Material& material, const utils::Vec4& RayDirection);
        bool Shadow (const utils::HitInfo& hitInfo, const utils::Vec4& lightDir);

        utils::Vec4 getPosition() const { return positionF; }
        utils::Vec4 getDirection() const { return directionF; }

        utils::RGB getIntensity() const { return intensityF; }
        utils::RGB getAmbient() const { return ambientLight; }
        utils::Vec4 getLightDir(utils::HitInfo hitInfo);
};
#endif // LIGHT_H
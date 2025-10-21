#ifndef LIGHT_H
#define LIGHT_H
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Cylinder.h"
#include "../headers/Cone.h"
#include "../headers/Flat.h"
#include "../headers/Ray.h"
#include "../headers/Scene.h"
#include <SDL.h>
#include <vector>


class Scene;

class Light {
    private:
        Scene* scene;
        utils::Vec4 positionF;
        utils::RGB intensityF;
        utils::RGB ambientLight;

    public:
        Light(float x, float y, float z, utils::RGB intensity, Scene* scene);
        utils::RGB ComputeLighting(const utils::HitInfo& hitInfo, const utils::Material& material, const utils::Vec4& RayDirection);
        bool Shadow (const utils::HitInfo& hitInfo, const utils::Vec4& lightDir);

        utils::Vec4 getPosition() const { return positionF; }
        utils::RGB getIntensity() const { return intensityF; }
        utils::RGB getAmbient() const { return ambientLight; }
};
#endif // LIGHT_H
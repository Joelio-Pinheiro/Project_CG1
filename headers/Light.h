#ifndef LIGHT_H
#define LIGHT_H
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include <SDL.h>

class Light {
    private:
        utils::Vec4 positionF;
        utils::Vec4 intensityF;

    public:
        Light(float x, float y, float z, utils::Vec4 intensity);
        utils::Vec4 ComputeLighting(const HitInfo& hitInfo, const Sphere* sphere, const utils::Vec4& RayDirection) const;

        utils::Vec4 getPosition() const { return positionF; }
        utils::Vec4 getIntensity() const { return intensityF; }
};
#endif // LIGHT_H
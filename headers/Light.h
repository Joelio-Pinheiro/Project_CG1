#ifndef LIGHT_H
#define LIGHT_H
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Flat.h"
#include "../headers/Ray.h"
#include <SDL.h>
#include <vector>

class Light {
    private:
        utils::Vec4 positionF;
        utils::RGB intensityF;
        utils::RGB ambientLight;

    public:
        Light(float x, float y, float z, utils::RGB intensity, utils::RGB ambient = {0.1f, 0.1f, 0.1f});
        utils::RGB ComputeLighting(const HitInfo& hitInfo, const Sphere* sphere, const std::vector<Sphere*>* spheres, const std::vector<Flat*>* flats, const utils::Vec4& RayDirection) const;
        utils::RGB ComputeLighting(const HitInfo& hitInfo, const Flat* flat, const std::vector<Sphere*>* spheres, const std::vector<Flat*>* flats, const utils::Vec4& RayDirection) const;
        bool Shadow (const HitInfo& hitInfo, const std::vector<Sphere*>* spheres, const std::vector<Flat*>* flats, const utils::Vec4& lightDir) const;

        utils::Vec4 getPosition() const { return positionF; }
        utils::RGB getIntensity() const { return intensityF; }
        utils::RGB getAmbient() const { return ambientLight; }
};
#endif // LIGHT_H
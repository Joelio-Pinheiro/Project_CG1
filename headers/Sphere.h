#ifndef SPHERE_H
#define SPHERE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"

struct HitInfo {
    bool hit; // Se ocorreu a interseção
    float t; // Distância ao longo do raio até a interseção
    utils::Vec4 point; // Ponto de interseção
    utils::Vec4 normal; // Normal da superfície na interseção
};

class Sphere {
    private:
        float radius;
        utils::Vec4 center;
        utils::RGB diffuse = {1.0f, 1.0f, 1.0f}; // Cores RGB da esfera
        utils::RGB specular = {1.0f, 1.0f, 1.0f};

    public:
        Sphere(float radius, float x, float y, float z);
        void setCenter(float x, float y, float z);
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        utils::RGB getDiffuse() const { return this->diffuse; };
        utils::RGB getSpecular() const { return this->specular; }
        utils::Vec4 getCenter() const;
        float getRadius();
        HitInfo intersects(const Ray& ray) const;
};

#endif // BALL_H
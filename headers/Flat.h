#ifndef FLAT_H
#define FLAT_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Ray.h"

// struct HitInfo2 {
//     bool hit; // Se ocorreu a interseção
//     float t; // Distância ao longo do raio até a interseção
//     utils::Vec4 point; // Ponto de interseção
//     utils::Vec4 normal; // Normal da superfície na interseção
// };
class Flat {
    private:
        utils::Vec4 point; // A point on the plane
        utils::Vec4 normal; // The normal vector of the plane

        utils::RGB diffuse = {1.0f, 1.0f, 1.0f}; // Diffuse color
        utils::RGB specular = {1.0f, 1.0f, 1.0f}; // Specular color
        float shininess = 10.0f; // Shininess factor for specular highlights

    public:
        Flat(const utils::Vec4& point, const utils::Vec4& normal);
        void setPoint(float x, float y, float z);
        void setNormal(float x, float y, float z);
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        utils::RGB getDiffuse() const { return this->diffuse; };
        utils::RGB getSpecular() const { return this->specular; }
        utils::Vec4 getPoint() const { return this->point; };
        utils::Vec4 getNormal() const { return this->normal; };
        HitInfo intersects(const Ray& ray) const;
};

#endif // FLAT_H
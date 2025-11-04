#ifndef FLAT_H
#define FLAT_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Ray.h"


class Flat {
    private:
        utils::Vec4 point; // A point on the plane
        utils::Vec4 normal; // The normal vector of the plane

        utils::Material material = utils::Material();

    public:
        Flat(const utils::Vec4& point, const utils::Vec4& normal);
        void setPoint(float x, float y, float z);
        void setNormal(float x, float y, float z);
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setMaterial(const utils::Material& mat) { this->material = mat; }
        void setShininess(float shininess) { this->material.setShininess(shininess); }
        utils::Material getMaterial() const { return this->material; };
        utils::Vec4 getPoint() const { return this->point; };
        utils::Vec4 getNormal() const { return this->normal; };
        utils::HitInfo intersects(const Ray& ray) const;
};

#endif // FLAT_H
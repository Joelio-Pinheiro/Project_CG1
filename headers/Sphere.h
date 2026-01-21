#ifndef SPHERE_H
#define SPHERE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"


class Sphere {
    private:
        float radius;
        utils::Vec4 center;
        utils::Material material = utils::Material();

    public:
        Sphere(float radius, float x, float y, float z);
        void setCenter(float x, float y, float z);
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setShininess(float shininess) { this->material.setShininess(shininess); }
        utils::Material getMaterial() const { return this->material; }
        utils::Vec4 getCenter() const;
        float getRadius();
        utils::HitInfo intersects(const Ray& ray) const;

        void scale(float s, const utils::Vec4& center) {
            radius *= s;
            this->center = center + (this->center - center) * s;
        };

        void translation(const utils::Vec4& t) {
            center = center + t;
        };
};

#endif // BALL_H
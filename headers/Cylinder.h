#ifndef CYLINDER_H
#define CYLINDER_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Flat.h"


class Cylinder {
    private:
        utils::Vec4 baseCenter; // Centro da base do cilindro    
        utils::Vec4 direction;  // Vetor eixo do cilindro (deve estar normalizado)
        float radius;           // Raio do cilindro
        float height;           // Altura do cilindro
        bool capped;          // Indica se o cilindro é fechado (com tampas) ou aberto

        utils::Material material = utils::Material();
    public:
        Cylinder(float radius, float height, utils::Vec4 baseCenter, utils::Vec4 direction, bool capped = true);
        void setBaseCenter(utils::Vec4 baseCenter);
        void setDirection(utils::Vec4 direction);
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setShininess(float shininess);
        void setMaterial(const utils::Material& m) { this->material = m; }
        utils::Material getMaterial() const { return this->material; }
        utils::Vec4 getBaseCenter() const { return baseCenter; };
        utils::Vec4 getDirection() const { return direction; };
        float getRadius() const { return radius; };
        float getHeight() const { return height; };
        bool isCapped() const { return capped; };
        utils::HitInfo intersects(const Ray& ray) const;

        void scale(float s, const utils::Vec4& center);
        void translation(const utils::Vec4& translation) {
            this->baseCenter = this->baseCenter + translation;
        }
};

#endif // CYLINDER_H
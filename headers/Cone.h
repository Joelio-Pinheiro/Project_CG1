#ifndef CONE_H
#define CONE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Flat.h"


class Cone {
    private:
        utils::Vec4 baseCenter; // Centro da base do cone
        utils::Vec4 direction;  // Vetor eixo do cone (deve estar normalizado)
        float radius;           // Raio do cone
        float height;           // Altura do cone
        bool capped;          // Indica se o cone é fechado (com tampa) ou aberto

        utils::Vec4 vertex;   // Ponto do ápice do cone (Vertice)

        utils::Material material = utils::Material();
    public:
        Cone(float radius, float height, utils::Vec4 baseCenter, utils::Vec4 direction, bool capped = true);
        Cone(float radius, utils::Vec4 baseCenter, utils::Vec4 vertex, bool capped = true);
        void setBaseCenter(utils::Vec4 baseCenter) { this->baseCenter = baseCenter; };
        void setDirection(utils::Vec4 direction) { this->direction = direction.normalize(); };
        void setHeight(float height) { this->height = std::abs(height); };
        void setRadius(float radius) { this->radius = radius; };
        void setVertex(utils::Vec4 vertex);
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        utils::Material getMaterial() const { return this->material; }
        utils::Vec4 getBaseCenter() const { return baseCenter; };
        utils::Vec4 getDirection() const { return direction; };
        utils::Vec4 getVertex() const { return vertex; };
        float getRadius() const { return radius; };
        float getHeight() const { return height; };
        bool isCapped() const { return capped; };
        utils::HitInfo intersects(const Ray& ray) const;
};

#endif // CONE_H
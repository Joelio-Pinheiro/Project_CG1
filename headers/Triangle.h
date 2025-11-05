#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"

class Triangle {
    private:
        utils::Vec4 p1; // vertice 0 ponto superior
        utils::Vec4 p2; // vertice 1 ponto inferior esquerdo
        utils::Vec4 p3; // vertice 2 ponto inferior direito

        utils::Material material = utils::Material();
    public:
        Triangle(utils::Vec4 p1, utils::Vec4 p2, utils::Vec4 p3);

        void setP1(utils::Vec4 p1) { this->p1 = p1; };
        void setP2(utils::Vec4 p2) { this->p2 = p2; };
        void setP3(utils::Vec4 p3) { this->p3 = p3; };
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setMaterial(const utils::Material& material) { this->material = material; }

        void translation(const utils::Vec4& translation);
        void scale(float s, const utils::Vec4& center);

        utils::Material getMaterial() const { return this->material; }
        utils::Vec4 getP1() const { return p1; };
        utils::Vec4 getP2() const { return p2; };
        utils::Vec4 getP3() const { return p3; };

        utils::HitInfo intersects(const Ray& ray) const;
};



#endif // TRIANGLE_H
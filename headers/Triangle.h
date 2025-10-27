#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"

class Triangle {
    private:
        utils::Vec4 v0; // vertice 0 ponto superior
        utils::Vec4 v1; // vertice 1 ponto inferior esquerdo
        utils::Vec4 v2; // vertice 2 ponto inferior direito

        utils::Vec4 e01; // Aresta v0 para v1
        utils::Vec4 e12; // Aresta v1 para v2
        utils::Vec4 e20; // Aresta v2 para v0

        utils::Material material = utils::Material();
    public:
        Triangle(utils::Vec4 v0, utils::Vec4 v1, utils::Vec4 v2);

        void setV0(utils::Vec4 v0) { this->v0 = v0; };
        void setV1(utils::Vec4 v1) { this->v1 = v1; };
        void setV2(utils::Vec4 v2) { this->v2 = v2; };
        void setE01(utils::Vec4 e01) { this->e01 = e01; };
        void setE12(utils::Vec4 e12) { this->e12 = e12; };
        void setE20(utils::Vec4 e20) { this->e20 = e20; };
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);

        utils::Material getMaterial() const { return this->material; }
        utils::Vec4 getV0() const { return v0; };
        utils::Vec4 getV1() const { return v1; };
        utils::Vec4 getV2() const { return v2; };

        utils::HitInfo intersects(const Ray& ray) const;
};



#endif // TRIANGLE_H
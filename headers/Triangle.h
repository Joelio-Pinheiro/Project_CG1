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
        float u1, u2, u3;
        float v1, v2, v3;
        bool hasUV = false;

        utils::Material material = utils::Material();
    public:
        Triangle(utils::Vec4 p1, utils::Vec4 p2, utils::Vec4 p3);

        void setP1(utils::Vec4 p1) { this->p1 = p1; };
        void setP2(utils::Vec4 p2) { this->p2 = p2; };
        void setP3(utils::Vec4 p3) { this->p3 = p3; };
        void setUVs(float u1, float u2, float u3, float v1, float v2, float v3) { this->u1 = u1; this->u2 = u2; this->u3 = u3; this->v1 = v1; this->v2 = v2; this->v3 = v3; hasUV = true; };
        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setMaterial(const utils::Material& material) { this->material = material; }

        void translation(const utils::Vec4& translation);
        void scale(float sx, float sy, float sz, const utils::Vec4& center);
        void shear(float shXY, float shXZ, float shYX, float shYZ, float shZX, float shZY, const utils::Vec4& center);
        void reflection(const utils::Vec4& planePoint, const utils::Vec4& planeNormal);
        void rotationX(float angle, const utils::Vec4& center);
        void rotationY(float angle, const utils::Vec4& center);
        void rotationZ(float angle, const utils::Vec4& center);
        void rotationAxisQuaternion(float angle, const utils::Vec4& axis, const utils::Vec4& center);

        utils::Material getMaterial() const { return this->material; }
        utils::Vec4 getP1() const { return p1; };
        utils::Vec4 getP2() const { return p2; };
        utils::Vec4 getP3() const { return p3; };
        bool usesUV() const { return hasUV; }

        utils::HitInfo intersects(const Ray& ray) const;
};



#endif // TRIANGLE_H
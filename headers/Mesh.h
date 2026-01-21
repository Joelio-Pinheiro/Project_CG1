#ifndef MESH_H
#define MESH_H
#include <SDL.h>
#include <GL/glew.h>
#include <vector>
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Triangle.h"


class Mesh {
    private:
        std::vector<Triangle*> triangles;
        std::vector<utils::Vec4> vertices;
        utils::Material material = utils::Material();
    public:
        Mesh() = default;
        Mesh(const std::vector<utils::Vec4>& verts, const std::vector<unsigned int>& indices);

        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setShininess(float shininess);
        void setMaterial(const utils::Material& material) { this->material = material; }

        utils::Material getMaterial() const { return this->material; }
        const std::vector<Triangle*>& getTriangles() const { return this->triangles; }

        void addTriangle(const utils::Vec4& p1, const utils::Vec4& p2, const utils::Vec4& p3);
        utils::HitInfo intersects(const Ray& ray) const;

        bool loadFromOBJ(const std::string& filepath);

        void translation(const utils::Vec4& t);
        void scale(float sx, float sy, float sz, const utils::Vec4& center);
        void rotationX(float angle, const utils::Vec4& center);
        void rotationY(float angle, const utils::Vec4& center);
        void rotationZ(float angle, const utils::Vec4& center);
        void rotationAxisQuaternion(float angle, const utils::Vec4& axis, const utils::Vec4& center);
        void shear(float shXY, float shXZ, float shYX, float shYZ, float shZX, float shZY, const utils::Vec4& center);
        void reflection(const utils::Vec4& planePoint, const utils::Vec4& planeNormal);

};

#endif // MESH_H
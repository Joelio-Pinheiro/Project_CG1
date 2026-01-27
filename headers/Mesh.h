#ifndef MESH_H
#define MESH_H
#include <SDL.h>
#include <GL/glew.h>
#include <vector>
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Triangle.h"

struct BVHNode {
    utils::AABB bounds;
    BVHNode* left = nullptr;
    BVHNode* right = nullptr;
    std::vector<Triangle*> tris; // apenas se for folha
    bool isLeaf() const { return left==nullptr && right==nullptr; }
};

class Mesh {
    private:
        std::vector<Triangle*> triangles;
        std::vector<utils::Vec4> vertices;
        utils::Material material = utils::Material();

        BVHNode* bvhRoot = nullptr;
        BVHNode* buildBVH(std::vector<Triangle*>& objs, int depth = 0, int maxLeafSize = 4);
        utils::HitInfo intersectsBVH(const Ray& ray, BVHNode* root) const;
        utils::AABB triangleAABB(const Triangle* tri) const;

    public:
        Mesh() = default;
        Mesh(const std::vector<utils::Vec4>& verts, const std::vector<unsigned int>& indices);

        void setDiffuse(float r, float g, float b);
        void setSpecular(float r, float g, float b);
        void setShininess(float shininess);
        void setMaterial(const utils::Material& material) { this->material = material; }

        utils::Material getMaterial() const { return this->material; }
        const std::vector<Triangle*>& getTriangles() const { return this->triangles; }

        void addTriangle(const utils::Vec4& p1, const utils::Vec4& p2, const utils::Vec4& p3, float u1=0, float u2=0, float u3=0, float v1=0, float v2=0, float v3=0);
        utils::HitInfo intersects(const Ray& ray) const;
        void build();
        void deleteBVH(BVHNode* node);

        bool loadFromOBJ(const std::string& filepath);

        void translation(const utils::Vec4& t);
        void scale(float sx, float sy, float sz, const utils::Vec4& center);
        void rotationX(float angle, const utils::Vec4& center);
        void rotationY(float angle, const utils::Vec4& center);
        void rotationZ(float angle, const utils::Vec4& center);
        void rotationAxisQuaternion(float angle, const utils::Vec4& axis, const utils::Vec4& center);
        void shear(float shXY, float shXZ, float shYX, float shYZ, float shZX, float shZY, const utils::Vec4& center);
        void reflection(const utils::Vec4& planePoint, const utils::Vec4& planeNormal);

        ~Mesh() {
            for (Triangle* t : triangles)
                delete t;
            deleteBVH(bvhRoot);
        }
};

#endif // MESH_H
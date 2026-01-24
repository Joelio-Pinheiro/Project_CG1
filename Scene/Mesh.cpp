#include "../headers/Mesh.h"
#include "../headers/utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>


Mesh::Mesh(const std::vector<utils::Vec4>& verts, const std::vector<unsigned int>& indices) {
    vertices = verts;
    for (size_t i = 0; i < indices.size(); i += 3) {
        utils::Vec4 p1 = vertices[indices[i]];
        utils::Vec4 p2 = vertices[indices[i + 1]];
        utils::Vec4 p3 = vertices[indices[i + 2]];
        addTriangle(p1, p2, p3);
    }
}
void Mesh::setDiffuse(float r, float g, float b) {
    this->material.setDiffuse(r, g, b);
}
void Mesh::setSpecular(float r, float g, float b) {
    this->material.setSpecular(r, g, b);
}
void Mesh::setShininess(float shininess) {
    this->material.setShininess(shininess);
}

void Mesh::addTriangle(const utils::Vec4& p1, const utils::Vec4& p2, const utils::Vec4& p3) {
    Triangle* triangle = new Triangle(p1, p2, p3);

    triangle->setMaterial(this->material);
    this->triangles.emplace_back(triangle);
}

// Transformações
void Mesh::translation(const utils::Vec4& t) {
    for (Triangle* triangle : this->triangles) {
        triangle->translation(t);
    }
}
void Mesh::scale(float sx, float sy, float sz, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->scale(sx, sy, sz, center);
    }
}
void Mesh::shear(float shXY, float shXZ, float shYX, float shYZ, float shZX, float shZY, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->shear(shXY, shXZ, shYX, shYZ, shZX, shZY, center);
    }
} 
void Mesh::reflection(const utils::Vec4& planePoint, const utils::Vec4& planeNormal) {
    for (Triangle* triangle : this->triangles) {
        triangle->reflection(planePoint, planeNormal);
    }
}
void Mesh::rotationX(float angle, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->rotationX(angle, center);
    }
}
void Mesh::rotationY(float angle, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->rotationY(angle, center);
    }
}
void Mesh::rotationZ(float angle, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->rotationZ(angle, center);
    }
}
void Mesh::rotationAxisQuaternion(float angle, const utils::Vec4& axis, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->rotationAxisQuaternion(angle, axis, center);
    }
}

utils::HitInfo Mesh::intersects(const Ray& ray) const {
    if (!this->bvhRoot) {
        utils::HitInfo hitInfo;
        hitInfo.hit = false;
        return hitInfo;
    } // BVH não construído
    return intersectsBVH(ray, this->bvhRoot);
}

bool Mesh::loadFromOBJ(const std::string& filepath) {
    std::vector<utils::Vec4> temp_vertices;
    std::vector<unsigned int> vertexIndices;

    std::ifstream file(filepath);
    if (!file.is_open()) { // caso de errado ao abrir o arquivo
        std::cerr << "Erro ao abrir o arquivo OBJ: " << filepath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            temp_vertices.emplace_back(utils::Vec4::Point(x, y, z));
        } else if (type == "f") {
            std::string s1,s2,s3;
            iss >> s1 >> s2 >> s3;
            auto parse = [](const std::string &tok)->unsigned int {
                std::istringstream t(tok);
                unsigned int idx=0;
                t >> idx;
                return idx;
            };
            unsigned int v1 = parse(s1), v2 = parse(s2), v3 = parse(s3);
            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);
        }
    }
    file.close();

    // limpar triângulos antigos (se houver)
    for (Triangle* t : triangles) delete t;
    triangles.clear();

    vertices = temp_vertices;
    triangles.reserve(vertexIndices.size()/3);

    for (size_t i=0; i<vertexIndices.size(); i+=3) {
        utils::Vec4 p1 = vertices[vertexIndices[i]];
        utils::Vec4 p2 = vertices[vertexIndices[i+1]];
        utils::Vec4 p3 = vertices[vertexIndices[i+2]];
        addTriangle(p1,p2,p3);
    }
    return true;
}

void Mesh::build() {
    if (bvhRoot) {deleteBVH(bvhRoot);}
    std::vector<Triangle*> objs = this->triangles;
    this->bvhRoot = buildBVH(objs, 0, 4);
}
void Mesh::deleteBVH(BVHNode* node) {
    if (!node) return;
    deleteBVH(node->left);
    deleteBVH(node->right);
    delete node;
}

utils::AABB Mesh::triangleAABB(const Triangle* tri) const {
    utils::AABB box;
    box.expand(tri->getP1());
    box.expand(tri->getP2());
    box.expand(tri->getP3());
    return box;
}

BVHNode* Mesh::buildBVH(std::vector<Triangle*>& objs, int depth, int maxLeafSize) {
    BVHNode* node = new BVHNode();

    //Calcular o AABB do nó atual
    for (Triangle* tri : objs) {
        node->bounds.expand(triangleAABB(tri));
    }

    //Se o número de triângulos for menor ou igual ao tamanho máximo da folha oun a profundidade máxima
    if ((int)objs.size() <= maxLeafSize || depth > 30) {
        node->tris = objs;
        return node;
    }

    // Escolher o eixo para dividir (0 = x, 1 = y, 2 = z)
    float extX = node->bounds.max.x - node->bounds.min.x;
    float extY = node->bounds.max.y - node->bounds.min.y;
    float extZ = node->bounds.max.z - node->bounds.min.z;
    int axis = (extX > extY && extX > extZ) ? 0 : (extY > extZ ? 1 : 2);

    std::sort(objs.begin(), objs.end(), [&](Triangle* a, Triangle* b){
        float ca = ( (axis==0 ? (a->getP1().x + a->getP2().x + a->getP3().x) :
                      (axis==1 ? (a->getP1().y + a->getP2().y + a->getP3().y) :
                                 (a->getP1().z + a->getP2().z + a->getP3().z)) ) / 3.0f);
        float cb = ( (axis==0 ? (b->getP1().x + b->getP2().x + b->getP3().x) :
                      (axis==1 ? (b->getP1().y + b->getP2().y + b->getP3().y) :
                                 (b->getP1().z + b->getP2().z + b->getP3().z)) ) / 3.0f);
        return ca < cb;
    });



    size_t mid = objs.size() / 2;
    std::vector<Triangle*> leftObjs(objs.begin(), objs.begin()+mid);
    std::vector<Triangle*> rightObjs(objs.begin()+mid, objs.end());

    node->left = buildBVH(leftObjs, depth+1, maxLeafSize);
    node->right = buildBVH(rightObjs, depth+1, maxLeafSize);

    return node;
}

utils::HitInfo Mesh::intersectsBVH(const Ray& ray, BVHNode* root) const {
    utils::HitInfo closest;
    closest.hit = false;
    closest.t = std::numeric_limits<float>::max();
    if (!root) return closest; // BVH vazio

    std::vector<BVHNode*> stack; // pilha para travessia iterativa
    stack.push_back(root); // iniciar na raiz
    while(!stack.empty()) {
        BVHNode* node = stack.back(); stack.pop_back();
        if (!utils::AABB::intersects(node->bounds, ray.getDirection(), ray.getOrigin(), closest.t)) continue; // não intersecta o AABB pois está além do mais próximo
        if (node->isLeaf()) {
            for (Triangle* tri : node->tris) {
                utils::HitInfo hi = tri->intersects(ray);
                if (hi.hit && hi.t < closest.t) {
                    closest = hi;
                }
            }
        } else {
            // push children (order by proximity could speedup)
            if (node->left) stack.push_back(node->left);
            if (node->right) stack.push_back(node->right);
        }
    }
    return closest;
}
#include "../headers/Mesh.h"
#include "../headers/utils.h"
#include <fstream>
#include <sstream>


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
void Mesh::scale(float s, const utils::Vec4& center) {
    for (Triangle* triangle : this->triangles) {
        triangle->scale(s, center);
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
    utils::HitInfo closestHit;
    closestHit.hit = false;
    closestHit.t = std::numeric_limits<float>::max();

    for (const Triangle* triangle : this->triangles) {
        utils::HitInfo hitInfo = triangle->intersects(ray);
        if (hitInfo.hit && hitInfo.t < closestHit.t) {
            closestHit = hitInfo;
        }
    }

    return closestHit;
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
            unsigned int v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            // OBJ usa os indices baseados em 1
            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);
        }
    }
    file.close();
    // Construir os triângulos a partir dos índices
    *this = Mesh(temp_vertices, vertexIndices);
    return true;
}
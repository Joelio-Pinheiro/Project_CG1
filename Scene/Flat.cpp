#include "../headers/Flat.h"
#include "../headers/utils.h"
#include "../headers/Sphere.h"
#include "../headers/Ray.h"

Flat::Flat(const utils::Vec4& point, const utils::Vec4& normal) {
    setPoint(point.x, point.y, point.z);
    setNormal(normal.x, normal.y, normal.z);
    setDiffuse(1.0f, 1.0f, 1.0f);
    setSpecular(1.0f, 1.0f, 1.0f);
}

void Flat::setPoint(float x, float y, float z) {
    this->point = utils::Vec4::Point(x, y, z);
}
void Flat::setNormal(float x, float y, float z) {
    this->normal = utils::Vec4::Vector(x, y, z).normalize();
}
void Flat::setDiffuse(float r, float g, float b) {
    this->diffuse = utils::RGB(r, g, b);
}
void Flat::setSpecular(float r, float g, float b) {
    this->specular = utils::RGB(r, g, b);
}

// HitInfo Flat::intersects(const Ray& ray) const {
//     HitInfo info;
//     info.hit = false;

//     float denom = this->normal.dot(ray.getDirection());
//     if (std::fabs(denom) > 1e-6) { // Evita divisão por zero
//         utils::Vec4 p0l0 = this->point - ray.getOrigin();
//         float t = p0l0.dot(this->normal) / denom;
//         if (t >= 0) {
//             info.hit = true;
//             info.t = t;
//             info.point = ray.position(t);
//             info.normal = this->normal;
//         }
//     }
//     return info;
// }

HitInfo Flat::intersects(const Ray& ray) const {
    HitInfo info;
    info.hit = false;

    float denom = ray.getDirection().dot(this->normal); // dr . n
    if (std::fabs(denom) > 1e-6) { // Evita divisão por zero
        utils::Vec4 p0l0 = ray.getOrigin() - this->getPoint(); // (P - P0)
        float t = p0l0.dot(this->normal) / denom; // (P - P0) . n / (dr . n)
        if (t >= 0) {
            info.hit = true;
            info.t = t;
            info.point = ray.position(t);
            info.normal = this->normal;
        }
    }
    return info;
}
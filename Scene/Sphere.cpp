#include "../headers/Sphere.h"
#include "../headers/utils.h"

Sphere::Sphere(float radius, float x, float y, float z) {
    this->radius = radius;
    this->setCenter(x, y, z);
}

void Sphere::setCenter(float x, float y, float z) {
    this->center = utils::Vec4::Point(x, y, z);
}
void Sphere::setcolors(int r, int g, int b) {
    this->colors = {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255};
}

utils::Vec4 Sphere::getCenter() {
    return center;
}

float Sphere::getRadius() {
    return radius;
}

HitInfo Sphere::intersects(const Ray& ray) {
    HitInfo info;
    info.hit = false;

    utils::Vec4 oc = ray.getOrigin().operator-(center); // (P - C) = w

    // formula da interseção raio-esfera
    // a*t^2 + b*t + c = 0
    float a = ray.getDirection().dot(ray.getDirection()); // dr . dr = 1
    float b = 2.0f * oc.dot(ray.getDirection()); // 2 * w . dr = 1.w
    float c = oc.dot(oc) - radius * radius; // w . w - r^2

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return info; // Sem interseção
    }

    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    float t = -1;
    if (t1 > 0 && t2 > 0) {
        t = std::min(t1, t2); // pega o mais próximo
    } else if (t1 > 0) {
        t = t1;
    } else if (t2 > 0) {
        t = t2;
    } 

    if (t > 0) {
        info.hit = true;
        info.t = t;
        info.point = ray.position(t);
        info.normal = (info.point.operator-(center)).normalize();
    }

    return info;
}

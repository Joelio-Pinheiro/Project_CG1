#include "../headers/Sphere.h"
#include "../headers/utils.h"

Sphere::Sphere(float radius, float x, float y, float z) {
    this->radius = radius;
    this->setCenter(x, y, z);
    setSpecular(1.0f, 1.0f, 1.0f);
}

void Sphere::setCenter(float x, float y, float z) {
    this->center = utils::Vec4::Point(x, y, z);
}
void Sphere::setDiffuse(float r, float g, float b) {
    this->material.setDiffuse(r, g, b);
}

void Sphere::setSpecular(float r, float g, float b) {
    this->material.setSpecular(r, g, b);
}

utils::Vec4 Sphere::getCenter() const {
    return center;
}

float Sphere::getRadius() {
    return radius;
}

utils::HitInfo Sphere::intersects(const Ray& ray) const{
    utils::HitInfo info;
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

    t -= 0.001f;

    if (t > 0) {
        info.hit = true;
        info.t = t;
        info.point = ray.position(t);
        info.normal = (info.point.operator-(center)).normalize();
        info.u = fmodf(info.point.x * 0.2f, 1.0f);
        info.v = fmodf(info.point.z * 0.2f, 1.0f);
        if (info.u < 0) info.u += 1.0f;
        if (info.v < 0) info.v += 1.0f;
    }
    info.type = "Sphere";

    return info;
}

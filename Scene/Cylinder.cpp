#include "../headers/Cylinder.h"
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Flat.h"

Cylinder::Cylinder(float radius, float height, utils::Vec4 baseCenter, utils::Vec4 direction, bool capped) {
    this->radius = radius;
    this->height = height;
    setBaseCenter(baseCenter);
    setDirection(direction);
    this->capped = capped;
}
void Cylinder::setBaseCenter(utils::Vec4 baseCenter) {
    this->baseCenter = baseCenter;
}
void Cylinder::setDirection(utils::Vec4 direction) {
    this->direction = direction.normalize();
}
void Cylinder::setDiffuse(float r, float g, float b) {
    this->material.setDiffuse(r, g, b);
}
void Cylinder::setSpecular(float r, float g, float b) {
    this->material.setSpecular(r, g, b);
}
void Cylinder::setShininess(float shininess) {
    this->material.setShininess(shininess);
}
void Cylinder::scale(float s, const utils::Vec4& center) {
    // Escala o raio e a altura
    this->radius *= s;
    this->height *= s;

    // Escala o centro da base em relação ao ponto center
    utils::Vec4 dirToBase = this->baseCenter - center;
    this->baseCenter = center + dirToBase * s;
}

utils::HitInfo Cylinder::intersects(const Ray& ray) const{
    utils::HitInfo info;
    info.hit = false;
    const float EPS = 1e-5f;

    // Cálculo da interseção raio-cilindro infinito
    utils::Vec4 rayDirection = ray.getDirection(); // dr
    utils::Vec4 rayOrigin = ray.getOrigin(); // P0
    utils::Vec4 dc = getDirection(); // vetor eixo do cilindro (dc)

    utils::Vec4 rayBaseCenter = rayOrigin - baseCenter; // P0 - C
    utils::Vec4 W = rayDirection - dc * rayDirection.dot(dc); // dr - (dr . dc) * dc
    utils::Vec4 V = rayBaseCenter - dc * rayBaseCenter.dot(dc); // (P0 - C) - ((P0 - C) . dc) * dc

    float a = W.dot(W); // w * w
    float b = 2 * (W.dot(V)); // 2 * (w . V)
    float c = V.dot(V) - radius * radius; // V . V - r^2

    
    if (std::abs(a) >= 1e-6) {
        // Raio não paralelo ao eixo do cilindro

        float discriminant = b * b - 4 * a * c; // Δ = b^2 - 4ac
        if (discriminant > 0.0f) {
            float sqrtDiscriminant = sqrt(discriminant);
            float t1 = (-b - sqrtDiscriminant) / (2 * a);
            float t2 = (-b + sqrtDiscriminant) / (2 * a);
            float candidates[2] = { t1, t2 };
            for (int i = 0; i < 2; ++i) {
                float t = candidates[i];
                t -= 0.001f;

                if (t <= EPS) continue; // Interseção atrás do raio
                    utils::Vec4 P = ray.position(t); // Ponto de interseção (PI) com o cilindro infinito
                    float y = (P - baseCenter).dot(dc); // Projeção de (PI - C) * dc no eixo do cilindro
                    if (y >= 0.0f - EPS && y <= height + EPS) { // Dentro dos limites do cilindro
                        if (!info.hit || t < info.t) {
                            info.hit = true;
                            info.t = t;
                            info.point = P;
                            
                            // normal na superfície do cilindro
                            utils::Vec4 proj = baseCenter + dc * y; // Projeção do PI no eixo do cilindro
                            info.normal = (P - proj).normalize(); // traça um vetor do eixo do cilindro até o ponto de interseção e normaliza
                        }
                    }

            }

        } 
    }
    if (isCapped()) {
        utils::Vec4 base = getBaseCenter();
        utils::Vec4 top  = getBaseCenter() + getDirection() * getHeight();

        struct Cap { Flat* flat; utils::Vec4 center; };
        Cap* caps[] = {
            new Cap{ new Flat(base, getDirection() * -1.0f), base },
            new Cap{ new Flat(top, getDirection()), top }
        };

        for (Cap* cap : caps) {
            utils::HitInfo capHit = cap->flat->intersects(ray);
            if (capHit.hit) {
                // verificar se dentro do círculo de raio
                if ((capHit.point - cap->center).length() <= radius + 1e-4f) {
                    if (!info.hit || capHit.t < info.t) {
                        info.hit = true;
                        info.t = capHit.t;
                        info.point = capHit.point;
                        info.normal = capHit.normal; // normal do plano (já apontando para fora)
                    }
                }
            }
        }
    }

    if (!info.hit) return info;

    if (info.normal.dot(rayDirection) > 0.0f) {
        info.normal = info.normal * -1.0f;
    }
    info.u = fmodf(info.point.x * 0.2f, 1.0f);
    info.v = fmodf(info.point.z * 0.2f, 1.0f);
    if (info.u < 0) info.u += 1.0f;
    if (info.v < 0) info.v += 1.0f;
    info.type = "Cylinder";
    return info;
}
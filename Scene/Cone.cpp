#include "../headers/Cone.h"
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include "../headers/Flat.h"

Cone::Cone(float radius, float height, utils::Vec4 baseCenter, utils::Vec4 direction, bool capped) {
    setRadius(radius);
    setBaseCenter(baseCenter);
    setDirection(direction);
    setHeight(height);
    setVertex(baseCenter + direction.normalize() * height);
    this->capped = capped;
}
Cone::Cone(float radius, utils::Vec4 baseCenter, utils::Vec4 vertex, bool capped) {
    setRadius(radius);
    setBaseCenter(baseCenter);
    setVertex(vertex);
    setDirection((vertex - baseCenter).normalize());
    setHeight((vertex - baseCenter).length());
    this->capped = capped;
}
void Cone::setVertex(utils::Vec4 vertex) {
    this->vertex = vertex;
}

void Cone::setDiffuse(float r, float g, float b) {
    this->material.setDiffuse(r, g, b);
}
void Cone::setSpecular(float r, float g, float b) {
    this->material.setSpecular(r, g, b);
}

// utils::HitInfo Cone::intersects(const Ray& ray) const {
//     utils::HitInfo hitInfo;
//     hitInfo.hit = false;

//     // Cálculo da interseção raio-cone
//     utils::Vec4 rayDirection = ray.getDirection().normalize(); // dr
//     utils::Vec4 rayOrigin = ray.getOrigin(); // P0
//     utils::Vec4 dc = getDirection().normalize(); // vetor eixo do cone (dc)
//     // utils::Vec4 w = getVertex() - rayOrigin; // V - P0
//     utils::Vec4 w = rayOrigin - getVertex(); // P0 - V



//     float tanTheta = radius / height; // tan(θ)
//     float cosTheta2 = 1.0f / (1.0f + tanTheta * tanTheta); // cos^2(θ)
//     float dpow = dc.dot(dc); // dc . dc
//     float DdotC = rayDirection.dot(dc);
//     float WdotC = w.dot(dc);

//     // float a = (rayDirection.dot(dc)) * (rayDirection.dot(dc)) - dpow * cosTheta2; // a = (dr . dc)^2 - (dc . dc)cos^2(theta)
//     float a = DdotC * DdotC - dpow * cosTheta2; // a = (dr . dc)^2 - (dc . dc)cos^2(theta)
//     float b = 2.0f * (w.dot(rayDirection)) * cosTheta2 - (WdotC * DdotC); // b = w . dr * cos^2(theta) - (w . dc) * (dr . dc)
//     // float b = 2.0f * (DdotC * WdotC - rayDirection.dot(w) * cosTheta2);
//     // float c = (w.dot(dc) * w.dot(dc)) - w.dot(w) * cosTheta2; // c = (w . dc)^2 - (w . w)cos^2(theta)
//     float c = WdotC * WdotC - w.dot(w) * cosTheta2;

//     float discriminant = b * b - 4 * a * c;

//     if (discriminant < 0) return hitInfo; // Sem interseção

//     float sqrtDiscriminant = sqrt(discriminant);
//     float t1 = (-b - sqrtDiscriminant) / (2 * a);
//     float t2 = (-b + sqrtDiscriminant) / (2 * a);

//     float t = std::min(t1, t2);
//     if (t < 0) t = std::max(t1, t2);
//     if (t < 0) return hitInfo; // Interseção atrás do raio

//     t -= 0.001f; // Evitar auto-interseção
//     utils::Vec4 PI = ray.position(t); // Ponto de interseção
//     float y = (PI - baseCenter).dot(dc); // Projeção de (PI - C) no eixo do cone
//     if (y < 0 || y > height) return hitInfo; // Interseção fora dos limites do cone
    
//     // normal da superfície
//     utils::Vec4 n = (PI - getVertex() - dc * y * (radius / height)).normalize();
    
//     hitInfo.hit = true;
//     hitInfo.point = PI;
//     hitInfo.t = t;
//     hitInfo.normal = n;

//     return hitInfo;
// }

utils::HitInfo Cone::intersects(const Ray& ray) const {
    utils::HitInfo hitInfo;
    hitInfo.hit = false;

    utils::Vec4 dr = ray.getDirection().normalize(); // Direção do raio normalizada (dr)
    utils::Vec4 P0 = ray.getOrigin(); // Origem do raio (P0)
    utils::Vec4 dc = getDirection().normalize(); // Vetor eixo do cone normalizado (dc)
    utils::Vec4 V = getVertex(); // Ponto do ápice do cone (V)

    utils::Vec4 w = P0 - V; // P0 - V

    float tanTheta = radius / height;
    float cosTheta2 = 1.0f / (1.0f + tanTheta * tanTheta);

    float DdotC = dr.dot(dc);
    float WdotC = w.dot(dc);

    float a = DdotC * DdotC - cosTheta2;
    float b = 2.0f * (DdotC * WdotC - dr.dot(w) * cosTheta2);
    float c = WdotC * WdotC - w.dot(w) * cosTheta2;

    float disc = b * b - 4 * a * c;
    if (disc < 0) return hitInfo;

    float sqrtDisc = sqrt(disc);
    float t1 = (-b - sqrtDisc) / (2 * a);
    float t2 = (-b + sqrtDisc) / (2 * a);

    float t = (t1 > 0) ? t1 : t2;
    if (t < 0) return hitInfo;

    t += 0.001f; // Evitar auto-interseção
    utils::Vec4 PI = ray.position(t);
    float y = (PI - baseCenter).dot(dc);

    if (y < 0 || y > height) return hitInfo;

    // normal da superfície
    utils::Vec4 n = (PI - V - dc * y * (radius / height)).normalize();

    hitInfo.hit = true;
    hitInfo.point = PI;
    hitInfo.t = t;
    hitInfo.normal = n;

    return hitInfo;
}

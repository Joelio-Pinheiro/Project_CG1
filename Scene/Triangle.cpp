#include "../headers/Triangle.h"
#include "../headers/utils.h"


Triangle::Triangle(utils::Vec4 p1, utils::Vec4 p2, utils::Vec4 p3) {
    setP1(p1);
    setP2(p2);
    setP3(p3);

}

void Triangle::setDiffuse(float r, float g, float b) {
    this->material.setDiffuse(r, g, b);
}
void Triangle::setSpecular(float r, float g, float b) {
    this->material.setSpecular(r, g, b);
}

void Triangle::translation(const utils::Vec4& translation) {
    setP1(getP1() + translation);
    setP2(getP2() + translation);
    setP3(getP3() + translation);
}
void Triangle::scale(float s, const utils::Vec4& center) {
    setP1((getP1() - center) * s + center);
    setP2((getP2() - center) * s + center);
    setP3((getP3() - center) * s + center);
}

utils::HitInfo Triangle::intersects(const Ray& ray) const {
    utils::HitInfo hitInfo;
    hitInfo.hit = false;

    // Coordenadas baricentricas
    utils::Vec4 p1 = getP1();
    utils::Vec4 p2 = getP2();
    utils::Vec4 p3 = getP3();

    // 1) Calcular os vetores de direção saindo de p1
    utils::Vec4 r1 = p2 - p1;
    utils::Vec4 r2 = p3 - p1;

    // 2) Calcular a normal do triângulo
    utils::Vec4 N = r2.prodVectorial(r1);
    utils::Vec4 normal = N.normalize();


    // 3) Calcular o ponto de interseção do raio com o plano do triângulo
    float denom = (ray.getDirection()).dot(normal); // dr . N
    if (std::abs(denom) < 1e-6) {
        return hitInfo; // O raio é paralelo ao plano do triângulo
    }
    float t = ((p1 - ray.getOrigin()).dot(normal)) / denom; // t = ( (P0 - P) . N ) / (dr . N)
    t -= 1e-4; // Evitar auto-interseção
    if (t < 0) {
        return hitInfo; // A interseção está atrás da origem do raio
    }
    utils::Vec4 PI = ray.position(t); // Ponto de interseção

    // 4) Calcular os vetores até o centro da interseção
    utils::Vec4 s1 = p1 - PI;
    utils::Vec4 s2 = p2 - PI;
    utils::Vec4 s3 = p3 - PI;

    utils::Vec4 V = PI - p1;

    // 5) Calcular as coordenadas baricentricas
    float areaTotal =  normal.dot(r1.prodVectorial(r2)); // Área do triângulo

    float C1 = (s3.prodVectorial(s1).dot(normal)) / areaTotal; // Área do sub-triângulo P1P2PI
    float C2 = (s1.prodVectorial(s2).dot(normal)) / areaTotal; // Área do sub-triângulo P2P3PI
    float C3 = (s2.prodVectorial(s3).dot(normal)) / areaTotal; // Área do sub-triângulo P3P1PI

    // 6) Verificar se o ponto de interseção está dentro do triângulo
    if (C1 >= 0 && C2 >= 0 && C3 >= 0 && (C1 + C2 + C3 <= 1.0f + 1e-6)) {
        if (normal.dot(ray.getDirection()) > 0) {
            normal = normal * -1.0f; // Inverter a normal se estiver apontando para dentro
        }
        hitInfo.hit = true;
        hitInfo.t = t;
        hitInfo.point = PI;
        hitInfo.normal = normal;
        hitInfo.u = fmodf(hitInfo.point.x * 0.2f, 1.0f);
        hitInfo.v = fmodf(hitInfo.point.z * 0.2f, 1.0f);
        if (hitInfo.u < 0) hitInfo.u += 1.0f;
        if (hitInfo.v < 0) hitInfo.v += 1.0f;
    }

    return hitInfo;
}
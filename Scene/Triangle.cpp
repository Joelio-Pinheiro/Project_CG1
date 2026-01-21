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

// Transformações
void Triangle::translation(const utils::Vec4& translation) {
    setP1(getP1() + translation);
    setP2(getP2() + translation);
    setP3(getP3() + translation);
}
void Triangle::scale(float sx, float sy, float sz, const utils::Vec4& center) {
    // Escala no P1
    utils::Vec4 p1 = getP1() - center;
    p1.x *= sx;
    p1.y *= sy;
    p1.z *= sz;
    setP1(p1 + center);

    // Escala no P2
    utils::Vec4 p2 = getP2() - center;
    p2.x *= sx;
    p2.y *= sy;
    p2.z *= sz;
    setP2(p2 + center);

    // Escala no P3
    utils::Vec4 p3 = getP3() - center;
    p3.x *= sx;
    p3.y *= sy;
    p3.z *= sz;
    setP3(p3 + center);
}
void Triangle::shear(float shXY, float shXZ, float shYX, float shYZ, float shZX, float shZY, const utils::Vec4& center) {
    // Cisalhamento no P1
    utils::Vec4 p1 = getP1() - center;
    float x = p1.x;
    float y = p1.y;
    float z = p1.z;

    p1.x = x + shXY * y + shXZ * z;
    p1.y = y + shYX * x + shYZ * z;
    p1.z = z + shZX * x + shZY * y;
    setP1(p1 + center);
    
    // Cisalhamento no P2
    utils::Vec4 p2 = getP2() - center;
    float x2 = p2.x;
    float y2 = p2.y;
    float z2 = p2.z;

    p2.x = x2 + shXY * y2 + shXZ * z2;
    p2.y = y2 + shYX * x2 + shYZ * z2;
    p2.z = z2 + shZX * x2 + shZY * y2;
    setP2(p2 + center);
    
    // Cisalhamento no P3
    utils::Vec4 p3 = getP3() - center;
    float x3 = p3.x;
    float y3 = p3.y;
    float z3 = p3.z;

    p3.x = x3 + shXY * y3 + shXZ * z3;
    p3.y = y3 + shYX * x3 + shYZ * z3;
    p3.z = z3 + shZX * x3 + shZY * y3;
    setP3(p3 + center);
}

void Triangle::reflection(const utils::Vec4& planePoint, const utils::Vec4& planeNormal) {
    utils::Vec4 N = planeNormal.normalize();
    utils::Vec4 P0 = planePoint;

    // reflection no P1
    utils::Vec4 p1 = getP1();
    float d1 = (p1 - P0).dot(N); // (p1 - P0) . N
    utils::Vec4 reflectedP1 = p1 - N * (2 * d1); // p1 - N * (2 * ((p1 - P0) . N))
    setP1(reflectedP1);

    // reflection no P2
    utils::Vec4 p2 = getP2();
    float d2 = (p2 - P0).dot(N);
    utils::Vec4 reflectedP2 = p2 - N * (2 * d2);
    setP2(reflectedP2);

    // reflection no P3
    utils::Vec4 p3 = getP3();
    float d3 = (p3 - P0).dot(N);
    utils::Vec4 reflectedP3 = p3 - N * (2 * d3);
    setP3(reflectedP3);
}

void Triangle::rotationX(float angle, const utils::Vec4& center) {
    float rad = angle * (M_PI / 180.0f);
    float cosA = cos(rad);
    float sinA = sin(rad);

    // Rotação no P1
    utils::Vec4 p1 = getP1() - center;
    float y1 = p1.y * cosA - p1.z * sinA;
    float z1 = p1.y * sinA + p1.z * cosA;
    p1.y = y1;
    p1.z = z1;
    setP1(p1 + center);

    // Rotação no P2
    utils::Vec4 p2 = getP2() - center;
    float y2 = p2.y * cosA - p2.z * sinA;
    float z2 = p2.y * sinA + p2.z * cosA;
    p2.y = y2;
    p2.z = z2;
    setP2(p2 + center);

    // Rotação no P3
    utils::Vec4 p3 = getP3() - center;
    float y3 = p3.y * cosA - p3.z * sinA;
    float z3 = p3.y * sinA + p3.z * cosA;
    p3.y = y3;
    p3.z = z3;
    setP3(p3 + center);
}

void Triangle::rotationY(float angle, const utils::Vec4& center) {
    float rad = angle * (M_PI / 180.0f);
    float cosA = cos(rad);
    float sinA = sin(rad);

    // Rotação no P1
    utils::Vec4 p1 = getP1() - center;
    float x1 = p1.z * sinA + p1.x * cosA;
    float z1 = p1.z * cosA - p1.x * sinA;
    p1.x = x1;
    p1.z = z1;
    setP1(p1 + center);

    // Rotação no P2
    utils::Vec4 p2 = getP2() - center;
    float x2 = p2.z * sinA + p2.x * cosA;
    float z2 = p2.z * cosA - p2.x * sinA;
    p2.x = x2;
    p2.z = z2;
    setP2(p2 + center);

    // Rotação no P3
    utils::Vec4 p3 = getP3() - center;
    float x3 = p3.z * sinA + p3.x * cosA;
    float z3 = p3.z * cosA - p3.x * sinA;
    p3.x = x3;
    p3.z = z3;
    setP3(p3 + center);
}

void Triangle::rotationZ(float angle, const utils::Vec4& center) {
    // float rad = angle * (M_PI / 180.0f);
    float cosA = cos(angle);
    float sinA = sin(angle);

    // Rotação no P1
    utils::Vec4 p1 = getP1() - center;
    float x1 = p1.x * cosA - p1.y * sinA;
    float y1 = p1.x * sinA + p1.y * cosA;
    p1.x = x1;
    p1.y = y1;
    setP1(p1 + center);

    // Rotação no P2
    utils::Vec4 p2 = getP2() - center;
    float x2 = p2.x * cosA - p2.y * sinA;
    float y2 = p2.x * sinA + p2.y * cosA;
    p2.x = x2;
    p2.y = y2;
    setP2(p2 + center);

    // Rotação no P3
    utils::Vec4 p3 = getP3() - center;
    float x3 = p3.x * cosA - p3.y * sinA;
    float y3 = p3.x * sinA + p3.y * cosA;
    p3.x = x3;
    p3.y = y3;
    setP3(p3 + center);
}

void Triangle::rotationAxisQuaternion(float angle, const utils::Vec4& axis, const utils::Vec4& center) {
    float rad = angle * (M_PI / 180.0f);

    utils::Quaternion q = utils::Quaternion::fromAxisAngle(axis, rad);

    // rotacionar P1
    utils::Vec4 p1 = getP1() - center;
    p1 = p1.rotateVec(q);
    setP1(p1 + center);

    // rotacionar P2
    utils::Vec4 p2 = getP2() - center;
    p2 = p2.rotateVec(q);
    setP2(p2 + center);

    // rotacionar P3
    utils::Vec4 p3 = getP3() - center;
    p3 = p3.rotateVec(q);
    setP3(p3 + center);
}

utils::HitInfo Triangle::intersects(const Ray& ray) const {
    utils::HitInfo hitInfo;
    hitInfo.hit = false;

    // Coordenadas baricentricas
    utils::Vec4 p1 = getP1();
    utils::Vec4 p2 = getP2();
    utils::Vec4 p3 = getP3();
    // std::cout << "Triangle P1: "; p1.print();
    // std::cout << "Triangle P2: "; p2.print();
    // std::cout << "Triangle P3: "; p3.print();

    // 1) Calcular os vetores de direção saindo de p1
    utils::Vec4 r1 = p2 - p1;
    // std::cout << "Triangle R1: "; r1.print();
    utils::Vec4 r2 = p3 - p1;
    // std::cout << "Triangle R2: "; r2.print();

    // 2) Calcular a normal do triângulo
    utils::Vec4 N = r1.prodVectorial(r2);
    utils::Vec4 normal = N.normalize();
    // std::cout << "Triangle Normal: "; normal.print();


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

    hitInfo.type = "Triangle";    
    return hitInfo;
}
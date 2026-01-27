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
void Cone::setShininess(float shininess) {
    this->material.setShininess(shininess);
}


utils::HitInfo Cone::intersects(const Ray& ray) const {
    utils::HitInfo hitInfo;
    hitInfo.hit = false;
    const float EPS = 1e-3f;

    utils::Vec4 dr = ray.getDirection().normalize(); // Direção do raio normalizada (dr)
    utils::Vec4 P0 = ray.getOrigin(); // Origem do raio (P0)
    utils::Vec4 dc = getDirection().normalize(); // Vetor eixo do cone normalizado (dc)
    utils::Vec4 V = getVertex(); // Ponto do ápice do cone (V)

    utils::Vec4 w = P0 - V; // P0 - V

    float tanTheta = radius / height;
    float cosTheta2 = 1.0f / (1.0f + tanTheta * tanTheta);

    float DdotC = dr.dot(dc);
    float WdotC = w.dot(dc);

    float a = DdotC * DdotC - cosTheta2; // a = (dr . dc)^2 - (dc . dc)cos^2(theta)
    // float b = 2.0f * (dr.dot(w) * cosTheta2 - DdotC * WdotC); // b = w . dr * cos^2(theta) - (w . dc) * (dr . dc)
    float b = 2.0f * (DdotC * WdotC - dr.dot(w) * cosTheta2); // b = w . dr * cos^2(theta) - (w . dc) * (dr . dc)
    float c = WdotC * WdotC - w.dot(w) * cosTheta2; // c = (w . dc)^2 - (w . w)cos^2(theta)


    if (std::abs(a) >= 1e-6) {
        float disc = b * b - 4 * a * c;
        if (disc > 0.0f) {
            float sqrtDisc = sqrt(disc);
            float t1 = (-b - sqrtDisc) / (2 * a);
            float t2 = (-b + sqrtDisc) / (2 * a);
            float candidates[2] = { t1, t2 };
            for (int i = 0; i < 2; ++i) {
                float t = candidates[i];
                t -= EPS; // Evitar auto-interseção
                if (t <= EPS) continue; // Interseção atrás do raio
                utils::Vec4 PI = ray.position(t);
                float y = (PI - baseCenter).dot(dc);
                if (y >= 0.0f - EPS && y <= height + EPS) { // Interseção fora dos limites do cone
                    if (!hitInfo.hit || t < hitInfo.t) {
                        hitInfo.hit = true;
                        hitInfo.t = t;
                        hitInfo.point = PI;

                        // normal da superfície
                        // utils::Vec4 n = (PI - V - dc * y * (radius / height)).normalize();
                        // utils::Vec4 n = ((PI - V).prodVectorial(dc).prodVectorial(PI - V)).normalize();
                        float k = radius / height;
                        utils::Vec4 pv = PI - V;
                        utils::Vec4 n = (pv - dc * ((1 + k * k) * pv.dot(dc))).normalize();
                        
                        n = n * -1.0f; 

                        hitInfo.normal = n;
                    }
                }
            }
        }
    }

    if(isCapped()) {
        // Verificar interseção com a base do cone
        Flat basecap = Flat(baseCenter, dc * -1.0f);
         utils::HitInfo baseHit = basecap.intersects(ray);
         if (baseHit.hit) {
             float yBase = (baseHit.point - baseCenter).length();
             if (yBase <= radius + 1e-4f) {
                 if (!hitInfo.hit || baseHit.t < hitInfo.t) {
                     hitInfo.hit = true;
                     hitInfo.t = baseHit.t;
                     hitInfo.point = baseHit.point;
                     hitInfo.normal = baseHit.normal;
                 }
             }
         }
    }
    if (!hitInfo.hit) return hitInfo;

    if (hitInfo.normal.dot(dr) > 0.0f) {
        hitInfo.normal = hitInfo.normal * -1.0f; 
    }

    hitInfo.point = hitInfo.point + hitInfo.normal * EPS;

    hitInfo.u = fmodf(hitInfo.point.x * 0.2f, 1.0f);
    hitInfo.v = fmodf(hitInfo.point.z * 0.2f, 1.0f);
    if (hitInfo.u < 0) hitInfo.u += 1.0f;
    if (hitInfo.v < 0) hitInfo.v += 1.0f;
    hitInfo.type = "Cone";

    return hitInfo;
}

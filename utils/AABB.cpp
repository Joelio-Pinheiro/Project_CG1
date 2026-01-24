#include "../headers/utils.h"

utils::AABB::AABB() {
    min = utils::Vec4(std::numeric_limits<float>::infinity(), 
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(), 1);
    max = utils::Vec4(-std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(), 1);
}

void utils::AABB::expand(const utils::Vec4& point) {
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);
    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);
}

void utils::AABB::expand(const AABB& other) {
    expand(other.min); 
    expand(other.max);
}

bool utils::AABB::intersects(const AABB& box, const utils::Vec4& dirRay, const utils::Vec4& originRay, float tMax) {
    float tMin = 0.0f;
    float tMaxLocal = tMax;

    for (int i = 0; i < 3; ++i) {
        float origin = (i==0? originRay.x : (i==1? originRay.y : originRay.z));
            float dir =    (i==0? dirRay.x: (i==1? dirRay.y: dirRay.z));
            float bmin =   (i==0? box.min.x : (i==1? box.min.y : box.min.z));
            float bmax =   (i==0? box.max.x : (i==1? box.max.y : box.max.z));
            if (std::abs(dir) < 1e-8f) {
                if (origin < bmin || origin > bmax) return false;
            } else {
                float invD = 1.0f / dir;
                float t0 = (bmin - origin) * invD;
                float t1 = (bmax - origin) * invD;
                if (t0 > t1) std::swap(t0,t1);
                tMin = std::max(tMin, t0);
                tMaxLocal = std::min(tMaxLocal, t1);
                if (tMaxLocal < tMin) return false;
            }
    }
    return true;
}
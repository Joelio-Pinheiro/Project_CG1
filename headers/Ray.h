#ifndef RAY_H
#define RAY_H
#include "../headers/utils.h"

class Ray {
    private:
        utils::Vec4 originPoint;
        utils::Vec4 dirVector;

    public:
        Ray(const utils::Vec4& origin, const utils::Vec4& direction);
        utils::Vec4 getOrigin() const;
        utils::Vec4 getDirection() const;
        utils::Vec4 position(float t) const;
};

#endif // RAY_H
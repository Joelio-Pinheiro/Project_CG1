#include "../headers/Ray.h"
#include "../headers/utils.h"
#include <SDL.h>
#include <GL/glew.h>

Ray::Ray(const utils::Vec4& origin, const utils::Vec4& direction) {
    this->originPoint = origin;
    this->dirVector = direction.normalize(); // Garante que o vetor direção é normalizado
}

utils::Vec4 Ray::getOrigin() const {
    return originPoint;
}

utils::Vec4 Ray::getDirection() const {
    return dirVector.normalize();
}

// P(t) = P0 + t * dr
utils::Vec4 Ray::position(float t) const {

    return originPoint + (dirVector * t);
}

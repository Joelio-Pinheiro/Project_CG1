#include "../headers/utils.h"
#include <SDL.h>

namespace utils
{

    RGB::RGB(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    RGB RGB::operator+(const RGB &other) const {
        return RGB(r + other.r, g + other.g, b + other.b);
    }
    RGB RGB::operator*(float k) const {
        return RGB(r * k, g * k, b * k);
    }

    SDL_Color RGB::toSDLColor() const {
        SDL_Color color;
        color.r = static_cast<Uint8>(clamp(r, 0.0f, 1.0f) * 255.0f);
        color.g = static_cast<Uint8>(clamp(g, 0.0f, 1.0f) * 255.0f);
        color.b = static_cast<Uint8>(clamp(b, 0.0f, 1.0f) * 255.0f);
        color.a = 255; 
        return color;
    }
    Vec4 RGB::toVec4() const {
        return Vec4::Vector(static_cast<float>(r), static_cast<float>(g), static_cast<float>(b));
    }
    RGB RGB::AtSign(const RGB &other) const {
        return RGB(
           (r * other.r),
           (g * other.g),
           (b * other.b));
    }
    float RGB::clamp(float v, float min, float max) {
        if (v < min)
            return min;
        if (v > max)
            return max;
        return v;
    }
}
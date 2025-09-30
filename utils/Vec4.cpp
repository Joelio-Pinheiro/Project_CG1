#include "../headers/utils.h"
#include <iostream>
#include <SDL.h>

namespace utils {
        // Construtor (padrão w = 0 → vetor)
        Vec4::Vec4(float x, float y, float z, float w) 
            : x(x), y(y), z(z), w(w) {}

        // Criar um ponto (x,y,z,1)
        Vec4 Vec4::Point(float x, float y, float z) {
            return Vec4(x, y, z, 1.0f);
        }

        // Criar um vetor (x,y,z,0)
        Vec4 Vec4::Vector(float x, float y, float z) {
            return Vec4(x, y, z, 0.0f);
        }

        // Soma (só faz sentido entre vetores ou ponto+vetor)
        Vec4 Vec4::operator+(const Vec4& v) const {
            return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        // Subtração (ponto - ponto = vetor, ponto - vetor = ponto, vetor - vetor = vetor)
        Vec4 Vec4::operator-(const Vec4& v) const {
            return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        // Multiplicação por escalar
        Vec4 Vec4::operator*(float k) const {
            return Vec4(x * k, y * k, z * k, w * k);
        }

        float Vec4::clamp(float v, float min, float max){
            if (v < min) return min;
            if (v > max) return max;
            return v;
        }

        // Multiplicação de rgb para vetores
        Vec4 Vec4::AtSign(const Vec4& v) const {
            return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
        }
        Vec4 Vec4::RGBConvert(SDL_Color color) const {
            return Vec4(color.r, color.g, color.b);
        }

        // Divisão por escalar
        Vec4 Vec4::operator/(float k) const {
            return Vec4(x / k, y / k, z / k, w / k);
        }

        // Produto escalar (dot) — ignora w
        float Vec4::dot(const Vec4& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        // Produto vetorial (cross) — só faz sentido para vetores (w=0)
        Vec4 Vec4::cross(const Vec4& v) const {
            return Vec4::Vector(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            );
        }

        // Norma (comprimento)
        float Vec4::length() const {
            return std::sqrt(x*x + y*y + z*z);
        }

        // Normalizar (apenas vetores, w=0)
        Vec4 Vec4::normalize() const {
            float len = length();
            if (len == 0) return Vec4::Vector(0,0,0);
            return (*this) / len;
        }

        // Impressão
        void Vec4::print() const {
            std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")\n";
        }
}
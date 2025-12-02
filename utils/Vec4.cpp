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

        // Divisão por escalar
        Vec4 Vec4::operator/(float k) const {
            return Vec4(x / k, y / k, z / k, w / k);
        }

        // Produto escalar (dot) — ignora w
        float Vec4::dot(const Vec4& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        // Produto vetorial (prodVectorial) — só faz sentido para vetores (w=0)
        Vec4 Vec4::prodVectorial(const Vec4& normal) const {
            return Vec4::Vector(
                y * normal.z - z * normal.y,
                z * normal.x - x * normal.z,
                x * normal.y - y * normal.x
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

        // Rotacionar vetor usando quaternion
        Vec4 Vec4::rotateVec(const Quaternion& q) const {
            utils::Quaternion pQuat(0, x, y, z);
            utils::Quaternion qConj = q.conjugate();
            
            // q * p
            Quaternion qp(
                q.w*pQuat.w - q.x*pQuat.x - q.y*pQuat.y - q.z*pQuat.z,
                q.w*pQuat.x + q.x*pQuat.w + q.y*pQuat.z - q.z*pQuat.y,
                q.w*pQuat.y - q.x*pQuat.z + q.y*pQuat.w + q.z*pQuat.x,
                q.w*pQuat.z + q.x*pQuat.y - q.y*pQuat.x + q.z*pQuat.w
            );

            // (q * p) * q⁻¹
            Quaternion result(
                qp.w*qConj.w - qp.x*qConj.x - qp.y*qConj.y - qp.z*qConj.z,
                qp.w*qConj.x + qp.x*qConj.w + qp.y*qConj.z - qp.z*qConj.y,
                qp.w*qConj.y - qp.x*qConj.z + qp.y*qConj.w + qp.z*qConj.x,
                qp.w*qConj.z + qp.x*qConj.y - qp.y*qConj.x + qp.z*qConj.w
            );

            return utils::Vec4(result.x, result.y, result.z);
        }
}
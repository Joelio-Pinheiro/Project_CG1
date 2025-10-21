#ifndef UTILS_H
#define UTILS_H
#include <SDL.h>
#include <GL/glew.h>  
#include <iostream>
#include <cmath>
#include <SDL.h>

namespace utils {
    
    struct Vec4 {
        float x, y, z, w;
        
        Vec4(float x=0, float y=0, float z=0, float w=0);
        static Vec4 Point(float x, float y, float z);
        static Vec4 Vector(float x, float y, float z);
        Vec4 operator+(const Vec4& v) const;
        Vec4 operator-(const Vec4& v) const;
        Vec4 operator*(float k) const;
        Vec4 operator/(float k) const;
        float dot(const Vec4& v) const;
        Vec4 prodVectorial(const Vec4& normal) const;
        Vec4 cross(const Vec4& v) const;
        float length() const;
        Vec4 normalize() const;
        void print() const;
    };
    
    struct HitInfo {
        bool hit; // Se ocorreu a interseção
        float t; // Distância ao longo do raio até a interseção
        utils::Vec4 point; // Ponto de interseção
        utils::Vec4 normal; // Normal da superfície na interseção
    };
    struct RGB {
        float r, g, b;

        static float clamp(float v, float min, float max);
        RGB(float r=0, float g=0, float b=0);
        RGB operator+(const RGB &other) const;
        RGB operator*(float k) const;
        SDL_Color toSDLColor() const;
        Vec4 toVec4() const;
        RGB AtSign(const RGB &other) const;
    };

    struct Material {
        utils::RGB diffuse;
        utils::RGB specular;
        utils::RGB ambient = this->diffuse;
        float shininess;

        Material() : diffuse(1.0f, 1.0f, 1.0f), specular(1.0f, 1.0f, 1.0f), shininess(10.0f) {}
        utils::RGB getDiffuse() const { return this->diffuse; }
        utils::RGB getSpecular() const { return this->specular; }
        utils::RGB getAmbient() const { return this->ambient; }
        float getShininess() const { return this->shininess; }
        void setDiffuse(float r, float g, float b) { this->diffuse = utils::RGB(r, g, b); }
        void setSpecular(float r, float g, float b) { this->specular = utils::RGB(r, g, b); }
        void setAmbient(float r, float g, float b) { this->ambient = utils::RGB(r, g, b); }
        void setShininess(float shininess) { this->shininess = shininess; }
    };

    class window {
        private:
            float WWindow;
            float HWindow;
            Vec4 position;
            SDL_Window *w;
            SDL_Renderer *renderer;
        public:
            window(float width, float height);
            ~window();

            float getWidth() { return this->WWindow; }
            float getHeight() { return this->HWindow; }
            Vec4 getPosition();
            void setPosition(float x, float y, float z) { this->position = Vec4::Point(x, y, z); }
            SDL_Renderer* getRenderer() { return this->renderer; }
        };

}
#endif 
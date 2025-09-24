#ifndef UTILS_H
#define UTILS_H
#include <SDL.h>
#include <GL/glew.h>  
#include <iostream>
#include <cmath> 

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
        Vec4 cross(const Vec4& v) const;
        float length() const;
        Vec4 normalize() const;
        void print() const;
    };

    class window {
        private:
            float WWindow;
            float HWindow;
            Vec4 position;
            SDL_Window *w;
        public:
            window(float width, float height);
            ~window();

            float getWidth() { return this->WWindow; }
            float getHeight() { return this->HWindow; }
            Vec4 getPosition();
            void setPosition(float x, float y, float z) { this->position = Vec4::Point(x, y, z); }
        };
}
#endif 
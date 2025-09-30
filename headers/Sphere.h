#ifndef BALL_H
#define BALL_H
#include <SDL.h>
#include <GL/glew.h>
#include "../headers/utils.h"
#include "../headers/Ray.h"
#include <vector>

struct HitInfo {
    bool hit; // Se ocorreu a interseção
    float t; // Distância ao longo do raio até a interseção
    utils::Vec4 point; // Ponto de interseção
    utils::Vec4 normal; // Normal da superfície na interseção
};

class Sphere {
    private:
        float radius;
        utils::Vec4 center;
        SDL_Color colors; // Cores RGB da esfera
        utils::Vec4 specular;

    public:
        Sphere(float radius, float x, float y, float z);
        void setCenter(float x, float y, float z);
        void setcolors(int r, int g, int b);
        SDL_Color getColors() { return this->colors; };
        utils::Vec4 getSpecular() const { return specular; }
        utils::Vec4 getCenter() const;
        float getRadius();
        HitInfo intersects(const Ray& ray);
        utils::Vec4 getColorsVec4() const;
};

#endif // BALL_H
#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../headers/utils.h"
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

// namespace utils { struct RGB; }

class Texture {
    private:
        int Width; // largura da textura
        int Height; // altura da textura

        SDL_Surface* surface; // ponteiro para a superfície SDL

    public:     
        Texture(const std::string& path);
        utils::RGB sample(float u, float v); // Amostra a cor na coordenada UV
        ~Texture();
};

#endif // TEXTURE_H
#include "../headers/utils.h"
#include <iostream>
#include <SDL.h>
#include <GL/glew.h>  

namespace utils
{
    window::window(float width, float height)
    {
         if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
            return;
        }
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "Erro ao inicializar SDL_image: " << IMG_GetError() << std::endl;
            return;
        }

        // setar tamanho
        this->WWindow = width;
        this->HWindow = height;

        // criar janela
        this->w = SDL_CreateWindow(
            "SDL2 Window",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            this->WWindow, this->HWindow,
            SDL_WINDOW_SHOWN);
        this->renderer = SDL_CreateRenderer(this->w, -1, SDL_RENDERER_ACCELERATED);

        if (!this->w) {
            std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
            SDL_Quit();
        }
        std::cout << "Window created with width: " << this->WWindow << " and height: " << this->HWindow << "\n";
    }

    window::~window() {
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->w);
        IMG_Quit();
        SDL_Quit();
    }

    Vec4 window::getPosition() {
        if (this->position.x || this->position.y || this->position.z) {
            return this->position;
        }
        return Vec4::Point(0, 0, 0);
    }
}
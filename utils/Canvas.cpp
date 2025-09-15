#include "../headers/utils.h"
#include <iostream>
#include <SDL.h>
#include <GL/glew.h>  

namespace utils
{
    canvas::canvas()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
            return;
        }
        this->WIDTH = 800;
        this->HEIGHT = 600;

        CreateWindow();
    }
    canvas::~canvas()
    {
        SDL_DestroyWindow(this->window);
        SDL_Quit();
    }

    void canvas::CreateWindow()
    {

        this->window = SDL_CreateWindow(
            "SDL2 Window + OpenGL",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            this->WIDTH, this->HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!this->window)
        {
            std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
            SDL_Quit();
        }
        std::cout << "Canvas created with width: " << this->WIDTH << " and height: " << this->HEIGHT << "\n";
    }

    void canvas::CreateContext()
    {
        this->glContext = SDL_GL_CreateContext(this->window);
        if (!this->glContext)
        {
            std::cerr << "SDL_GL_CreateContext error: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(this->window);
            SDL_Quit();
        }

        if (glewInit() != GLEW_OK){
            std::cerr << "Erro ao inicializar GLEW!" << std::endl;
        }
    }

}
#include <SDL.h>
#include <iostream>
#include "headers/utils.h"
#include <GL/glew.h>  
#include <vector>


int main(int argc, char **argv)
{
    utils::window myCanvas(800, 600);
    std::cout << "Window dimensions: " << myCanvas.getPosition().x << ", " << myCanvas.getPosition().y << ", " << myCanvas.getPosition().z << "\n";


    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
            if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_g) {
                std::cout << "G key released\n";
            }
            
        }
        glClear(GL_COLOR_BUFFER_BIT);

    }

    myCanvas.~window();
    return 0;
}

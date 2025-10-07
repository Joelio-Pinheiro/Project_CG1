#include <SDL.h>
#include <iostream>
#include "headers/utils.h"
#include "headers/Scene.h"
#include <GL/glew.h>  
#include <vector>


int main(int argc, char **argv)
{   
    float widthPixels = 500.0f;
    float heightPixels = 500.0f;
    utils::window myWindow(widthPixels, heightPixels);
    Scene myScene(60.0f, 60.0f, -30.0f, widthPixels, heightPixels, &myWindow);


    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
            
        }
        myScene.render();
        
    }

    myWindow.~window();
    return 0;
}

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

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w) myScene.incrementObserverZ(1.0f);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s) myScene.decrementObserverZ(1.0f);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d) myScene.incrementObserverX(1.0f);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a) myScene.decrementObserverX(1.0f);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) myScene.incrementObserverY(1.0f);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) myScene.decrementObserverY(1.0f);
            printf("Observer Position: (%.2f, %.2f, %.2f)\n", myScene.getObserverPosition().x, myScene.getObserverPosition().y, myScene.getObserverPosition().z);
        }
        myScene.render();
        
    }

    myWindow.~window();
    return 0;
}

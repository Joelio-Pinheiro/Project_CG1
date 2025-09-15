#include <SDL.h>
#include <iostream>
#include "headers/utils.h"
#include <GL/glew.h>  
#include <vector>


int main(int argc, char **argv)
{
    std::vector<std::pair<float, float>> points;
    utils::canvas myCanvas;

    myCanvas.CreateContext();

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
            if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_g) {
                std::cout << "G key released\n";
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                std::cout << "(" << x << ", " << y << ")\n";

                int gly = myCanvas.getHeight() - y;

                float ndx = (2.0f * x) / myCanvas.getWidth() - 1.0f;
                float ndy = (2.0f * gly) / myCanvas.getHeight() - 1.0f;
                points.push_back({ndx, ndy});
            }
            
        }
        // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        for (std::pair<float, float> point : points) {
            glVertex2f(point.first, point.second);
        }
        glEnd();

        SDL_GL_SwapWindow(myCanvas.window);
    }

    //     Clear (black)
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //     SDL_RenderClear(renderer);

    //     // White rectangle in center
    //     SDL_Rect r;
    //     r.w = 300; r.h = 200;
    //     r.x = (WIDTH - r.w) / 2;
    //     r.y = (HEIGHT - r.h) / 2;
    //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //     SDL_RenderFillRect(renderer, &r);

    //     SDL_RenderPresent(renderer);
    //     SDL_Delay(16); // ~60 FPSh
    // }

    // SDL_DestroyRenderer(renderer);
    myCanvas.~canvas();
    return 0;
}

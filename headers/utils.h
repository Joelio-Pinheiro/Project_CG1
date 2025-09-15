#ifndef UTILS_H
#define UTILS_H
#include <SDL.h>
#include <GL/glew.h>  

namespace utils {
    class canvas {
        private:
            int WIDTH;
            int HEIGHT;
        public:
            SDL_Window *window;
            SDL_GLContext glContext;
            canvas();
            ~canvas();
            void CreateWindow();
            void CreateContext();
            void DestroyContext() { SDL_GL_DeleteContext(this->glContext); }
            int getWidth() { return this->WIDTH; }
            int getHeight() { return this->HEIGHT; }
        };
}
#endif 
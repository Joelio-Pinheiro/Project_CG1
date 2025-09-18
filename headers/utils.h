#ifndef UTILS_H
#define UTILS_H
#include <SDL.h>
#include <GL/glew.h>  

namespace utils {
    struct Position {
        float x;
        float y;
        float z;
    };
    class window {
        private:
            int WWindow;
            int HWindow;
            Position position;
            SDL_Window *w;
        public:
            window(int width, int height);
            ~window();

            int getWidth() { return this->WWindow; }
            int getHeight() { return this->HWindow; }
            Position getPosition() { return this->position; }
            void setPosition(float x, float y, float z) { this->position.x = x; this->position.y = y; this->position.z = z; }
        };
}
#endif 
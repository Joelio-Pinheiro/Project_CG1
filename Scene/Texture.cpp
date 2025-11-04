#include "../headers/Texture.h"
#include <iostream>
#include <algorithm>

Texture::Texture(const std::string& path) {
    surface = IMG_Load(path.c_str());     // pode ser PNG se usar SDL_image
    if (!surface) {
        std::cerr << "Erro ao carregar textura: " << SDL_GetError() << std::endl;
        Width = Height = 0;
        return;
    }
    std::cout << "Textura carregada: " << path << " (" << surface->w << "x" << surface->h << ")\n";
    Width  = surface->w;
    Height = surface->h;
}



utils::RGB Texture::sample(float u, float v) {
    if (!surface) return utils::RGB(1.0f, 0.0f, 1.0f); // Retorna rosa se a textura não foi carregada

    // int x = std::min(Width  - 1, std::max(0, int(u * Width)));
    // int y = std::min(Height - 1, std::max(0, int(v * Height)));

    // Uint32* pixels = (Uint32*)surface->pixels;
    // Uint32 pixel = pixels[y * Width + x];

    // utils::RGB color;
    // SDL_GetRGB(pixel, surface->format,
    //            (Uint8*)&color.r,
    //            (Uint8*)&color.g,
    //            (Uint8*)&color.b);
    // color.r /= 255.0f;
    // color.g /= 255.0f;
    // color.b /= 255.0f;

    // return color;

    int x = std::clamp(int(u * Width),  0, Width  - 1);
    int y = std::clamp(int(v * Height), 0, Height - 1);

    // Pega o primeiro byte do pixel (como char*)
    Uint8* pixelPtr = (Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    Uint32 pixel = 0;

    // copia o pixel para um Uint32 (sem assumir formato)
    memcpy(&pixel, pixelPtr, surface->format->BytesPerPixel);

    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

    return utils::RGB(r / 255.0f, g / 255.0f, b / 255.0f);
}


Texture::~Texture() {
    if (surface) {        
        SDL_FreeSurface(surface);
    }
}
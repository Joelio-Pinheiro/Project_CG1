#include <SDL.h>
#include <iostream>
#include "headers/utils.h"
#include "headers/Scene.h"
#include <GL/glew.h>  
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"


int main(int argc, char **argv)
{   
    float widthPixels = 500.0f;
    float heightPixels = 500.0f;
    utils::window myWindow(widthPixels, heightPixels);
    Scene myScene(60.0f, 60.0f, 30.0f, widthPixels, heightPixels, &myWindow);

    // Interface IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Habilitar navegação por teclado
    ImGui::StyleColorsDark();

    SDL_Window* sdlWindow = myWindow.getSDLWindow();
    SDL_Renderer* renderer = myWindow.getRenderer();

    ImGui_ImplSDL2_InitForSDLRenderer(sdlWindow, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool running = true;
    SDL_Event e;
    while (running) {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e); // passar eventos para o ImGui
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
            myScene.handleEvent(e);
            myScene.handleKeyboard(keys);
        }


        // Iniciar o frame do ImGui
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        
        // Renderizar a cena
        myScene.render();
        
        // Controles laterais UI
        // ImGui::Begin("Scene Controls");
        // ImGui::Text("Camera");
        // ImGui::Separator();
        // ImGui::Text("Click on the scene to pick objects.");
        // ImGui::End();


        // Renderizar o ImGui
        SDL_Texture* tex = myScene.getCanvasTexture();
        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (tex) {
            SDL_RenderCopy(renderer, tex, nullptr, nullptr);
        }


        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

         // Renderizar a cena
        SDL_RenderPresent(renderer);
        
    }   
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return 0;
}

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
    const int menuWidth = 240;
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

        // janela lateral
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)heightPixels), ImGuiCond_Always);
        ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        ImGui::Begin("Cena Controls", nullptr, winFlags);
        
        // Speed render control
        int speed = myScene.getSpeedRender();
        if (ImGui::SliderInt("Speed Render", &speed, 1, 60)) {
            myScene.setSpeedRender(speed);
            myScene.markDirty();
        }
        ImGui::Text("Current speed: %d", myScene.getSpeedRender());

        // Camera info & controls
        Camera* cam = myScene.getCamera();
        if (cam) {
            utils::Vec4 eye = cam->getEye();
            utils::Vec4 fwd = cam->getForward();
            utils::Vec4 up = cam->getUp();
            utils::Vec4 right = cam->getRight();

            // Mostrar (readonly) e editar (InputFloat3) valores
            float eyeArr[3] = { eye.x, eye.y, eye.z };
            if (ImGui::InputFloat3("Eye (x,y,z)", eyeArr)) {
                cam->setEye(utils::Vec4::Point(eyeArr[0], eyeArr[1], eyeArr[2]));
                myScene.markDirty();
            }

            float fwdArr[3] = { fwd.x, fwd.y, fwd.z };
            if (ImGui::InputFloat3("Forward (x,y,z)", fwdArr)) {
                cam->setForward(utils::Vec4::Vector(fwdArr[0], fwdArr[1], fwdArr[2]));
                myScene.markDirty();
            }

            float upArr[3] = { up.x, up.y, up.z };
            if (ImGui::InputFloat3("Up (x,y,z)", upArr)) {
                cam->setUp(utils::Vec4::Vector(upArr[0], upArr[1], upArr[2]));
                myScene.markDirty();
            }

            ImGui::Text("Right: (%.2f, %.2f, %.2f)", right.x, right.y, right.z);

            // Projection selector
            const char* projNames[] = { "Perspective", "Orthographic", "Oblique" };
            int curProj = (int)cam->getProjection();
            if (ImGui::Combo("Projection", &curProj, projNames, IM_ARRAYSIZE(projNames))) {
                cam->setProjection((ProjectionType)curProj);
                myScene.markDirty();
            }

            // Quick camera ops
            if (ImGui::Button("Reset Camera")) {
                cam->setEye(utils::Vec4::Point(0.0f, 1.6f, -2.6f));
                cam->setForward(utils::Vec4::Vector(0.0f, 0.7f, 1.0f));
                cam->setUp(utils::Vec4::Vector(0.0f, 1.0f, 0.0f));
                myScene.markDirty();
            }
        }

        ImGui::Spacing();
        ImGui::TextWrapped("Use este painel para controlar velocidade e parâmetros da câmera.\nAs mudanças são aplicadas em tempo real.");

        ImGui::End();

        // Renderizar a cena
        if (myScene.isDirty()) {
            myScene.render();
            myScene.clearDirty();
        }


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

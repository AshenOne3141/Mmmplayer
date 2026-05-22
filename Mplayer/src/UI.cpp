#include "../third_party/imgui/imgui.h"

#include "../third_party/imgui/backends/imgui_impl_sdl3.h"

#include "../third_party/imgui/backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <windows.h>
#include <commdlg.h>
#include "../include/UI.h"
#include <iostream>
bool UI::initialise(
    SDL_Window* window,
    SDL_Renderer* renderer

) {
    this->renderer = renderer;
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(
        window,
        renderer
    );

    ImGui_ImplSDLRenderer3_Init(renderer);

    return true;
}
void UI::beginFrame() {

    ImGui_ImplSDLRenderer3_NewFrame();

    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();

}
void UI::shutdown() {

    ImGui_ImplSDLRenderer3_Shutdown();

    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();
}
void UI::processEvents(SDL_Event* event) {

    ImGui_ImplSDL3_ProcessEvent(event);
}
std::string UI::render(bool& running) {

    std::string selectedFile = "";

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {

            if (ImGui::MenuItem("Open")) {

                OPENFILENAMEA ofn;

                char szFile[260];

                ZeroMemory(&ofn, sizeof(ofn));

                ofn.lStructSize = sizeof(ofn);

                ofn.lpstrFile = szFile;

                ofn.lpstrFile[0] = '\0';

                ofn.nMaxFile = sizeof(szFile);

                ofn.lpstrFilter =
                    "Video Files\0*.mp4;*.mkv;*.avi\0";

                ofn.Flags =
                    OFN_PATHMUSTEXIST |
                    OFN_FILEMUSTEXIST;

                if (GetOpenFileNameA(&ofn)) {

                    selectedFile = szFile;
                }
            }

            if (ImGui::MenuItem("Exit")) {

                running = false;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(
        ImGui::GetDrawData(),
        renderer
    );

    return selectedFile;
}
/*
    TODO:
    FINISH COMPONENT SYSTEM
    AUTO DOCKING
    FINISH THE CMD DEBUGGING

*/


#include "raylib.h"
#include "Engine.h"
#include "Panel.h" 
#include "Camera2D.h"
#include "GameObject.h"
#include <iostream>
#include "imgui.h"
#include "rlImGui.h"
#include "Grid.h"
#include "rlgl.h"
#include "imgui_impl_raylib.h"
#include "Component.h"

#define DEBUG_RESET   "\033[0m"
#define DEBUG_RED     "\033[31m"
#define DEBUG_GREEN   "\033[32m"
#define DEBUG_YELLOW  "\033[33m"
#define DEBUG_BLUE    "\033[34m"
#define DEBUG_MAGENTA "\033[35m"
#define DEBUG_CYAN    "\033[36m"
#define DEBUG_WHITE   "\033[37m"

Camera2DController cameraController;

//ImFont* pFont;

void PrintDebug(const std::string& message) {
    std::cout << " Xethium Engine >> ";
    std::cout << DEBUG_BLUE << "[DEBUG] " << DEBUG_RESET << message << std::endl;
}

void PrintInfo(const std::string& message) {
    std::cout << " Xethium Engine >> ";
    std::cout << DEBUG_GREEN << "[INFO] " << DEBUG_RESET << message << std::endl;
}

void PrintWarning(const std::string& message) {
    std::cout << " Xethium Engine >> ";
    std::cout << DEBUG_YELLOW << "[WARNING] " << DEBUG_RESET << message << std::endl;
}

void PrintError(const std::string& message) {
    std::cout << " Xethium Engine >> ";
    std::cout << DEBUG_RED << "[ERROR] " << DEBUG_RESET << message << std::endl;
}

bool Engine::Init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_ERROR);

    PrintInfo("Initializing Window...");
    InitWindow(1200, 600, "Xethium Engine");
    int winX = GetMonitorWidth(GetCurrentMonitor()), winY = GetMonitorHeight(GetCurrentMonitor());
    int setWinX = winX / 2 + 100, setWinY = winY / 2 + 100;
    SetWindowSize(setWinX, setWinY);

    cameraController.Init();
    PrintInfo("Initializing Camera...");

    PrintInfo("Initializing GUI...");
    rlImGuiSetup(true); // Initialize rlImGui
    panel.Init();
#ifdef IMGUI_HAS_DOCK
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    return !WindowShouldClose();
}

void Engine::Run() {
    while (!WindowShouldClose()) {
        Update();
        Renderer();
    }
}

void Engine::Shutdown() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
    panel.Shutdown();
    CloseWindow();
}

void Engine::Update() {
    cameraController.Update();
    cube.Update();
    grid.Update();
    panel.Update();
    printf("");
}

void Engine::Renderer() {
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    //ImGui::NewFrame();

    // Begin drawing to the screen
    BeginDrawing();
    ClearBackground(BLACK);

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    // Prepare to render to a texture (for the viewport)
    BeginTextureMode(panel.ViewportTexture);
    ClearBackground(DARKGRAY);

    // Begin drawing in 2D mode for the game objects
    BeginMode2D(cameraController.GetCamera());
    //cube.Render();
    Component::RenderEntities();
    grid.Render(cameraController.GetCamera());
    EndMode2D();

    // End rendering to the texture
    EndTextureMode();

    // Start a new ImGui frame
    rlImGuiBegin();  // Begin ImGui rendering with Raylib

#ifdef IMGUI_HAS_DOCK
    ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);
#endif

    //panel.EntityPropertyWindow();

    // Render ImGui windows
    if (ImGui::Begin("Viewport")) {
        rlImGuiImageRenderTextureFit(&(panel.ViewportTexture), true);
    }
    ImGui::End();

    if (ImGui::Begin("Inspector")) {
        ImGui::InputInt("Input ID: ", &Component::selectedEntity);
        if (Component::selectedEntity >= 0 && Component::selectedEntity < Component::entityCount) {
            Entity& entity = Component::entities[Component::selectedEntity];

            ImGui::TextUnformatted("Cube Position:\n");
            ImGui::TextUnformatted("\n");
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::TextUnformatted("X:");
            ImGui::DragFloat("Position X-axis", (float*)&entity.position.x, 2.0f, 1.0f);
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
            ImGui::TextUnformatted("Y:");
            ImGui::DragFloat("Position Y-axis", (float*)&entity.position.y, 2.0f, 1.0f);
            ImGui::PopStyleColor();
            
            ImGui::Text("Cube Size:");
            ImGui::Text("\n");
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::TextUnformatted("X:");
            ImGui::DragFloat("Size X-axis", (float*)&entity.size.x, 2.0f, 1.0f);
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
            ImGui::TextUnformatted("Y:");
            ImGui::DragFloat("Size Y-axis", (float*)&entity.size.y, 2.0f, 1.0f);
            ImGui::PopStyleColor();
            
            float color[4]{
                entity.color.r / 255.0f,
                entity.color.g / 255.0f,
                entity.color.b / 255.0f,
                entity.color.a / 255.0f,
            };

            if (ImGui::ColorEdit3("Color", color)) {
                entity.color.r = (unsigned char)(color[0] * 255.0f);
                entity.color.g = (unsigned char)(color[1] * 255.0f);
                entity.color.b = (unsigned char)(color[2] * 255.0f);
                entity.color.a = (unsigned char)(color[3] * 255.0f);
            }

        }
    }
    ImGui::End();

    if (ImGui::Begin("Components")) {
        if (ImGui::Button("Add Component")) {
            Component::CreateEntity({ 50, 50 }, { 100, 100 }, RED);
        }
    }
    ImGui::End();


    if (ImGui::Begin("File Manager")) {
        ImGui::TextUnformatted("This is where you manage files.");
    }
    ImGui::End();


   


    // End ImGui rendering with Raylib
    rlImGuiEnd();
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
    EndDrawing();
}

void Engine::Render() {
    // Additional render logic if necessary
}

#define IMGUI_HAS_DOCK
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
#include "imgui_internal.h"

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

bool isTextured = false;
char texturePathBuffer[256] = "";

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
    grid.Update();
    panel.Update();
    printf("");
}

void Engine::Renderer() {
    Texture2D texture = LoadTextureFromCache(texturePathBuffer);
    Vector2 mousePos = GetMousePosition();

    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();

    BeginDrawing();
    ClearBackground(BLACK);

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    BeginTextureMode(panel.ViewportTexture);
    ClearBackground(DARKGRAY);

    BeginMode2D(cameraController.GetCamera());
    grid.Render(cameraController.GetCamera());
    RenderEntities();
    EndMode2D();

    EndTextureMode();

    rlImGuiBegin();

    ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
    static ImVec2 previous_size = ImVec2(0, 0);
    if (viewport_size.x != previous_size.x || viewport_size.y != previous_size.y) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport_size);
        previous_size = viewport_size;
    }

#ifdef IMGUI_HAS_DOCK
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, NULL, ImGuiDockNodeFlags_PassthruCentralNode);
#endif

    panel.SetupDocking();

    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        rlImGuiImageRenderTextureFit(&(panel.ViewportTexture), true);
    }
    ImGui::End();

    if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (selectedEntity >= 0 && selectedEntity < entities.size()) {
            Entity& entity = entities[selectedEntity];

            if (ImGui::CollapsingHeader("Position"))
            {
                ImGui::Text("Cube Position:");
                ImGui::DragFloat("Position X-axis", &entity.position.x, 2.0f, 1.0f);
                ImGui::DragFloat("Position Y-axis", &entity.position.y, 2.0f, 1.0f);

                ImGui::Text("Cube Size:");
                ImGui::DragFloat("Size X-axis", &entity.size.x, 2.0f, 1.0f);
                ImGui::DragFloat("Size Y-axis", &entity.size.y, 2.0f, 1.0f);

                float color[3] = {
                    entity.color.r / 255.0f,
                    entity.color.g / 255.0f,
                    entity.color.b / 255.0f
                };
                if (ImGui::ColorEdit3("Color", color)) {
                    entity.color.r = (unsigned char)(color[0] * 255.0f);
                    entity.color.g = (unsigned char)(color[1] * 255.0f);
                    entity.color.b = (unsigned char)(color[2] * 255.0f);
                }
            }
            if (ImGui::CollapsingHeader("Texture")) {
                ImGui::Checkbox("Is Textured", &isTextured);
                    if (isTextured) {
                        ImGui::InputText("Texture Path", texturePathBuffer, sizeof(texturePathBuffer));

                        if (ImGui::Button("Apply Texture")) {
                            if (entity.hasTexture) {
                                UnloadTexture(entity.texture);
                            }
                            entity.texture = LoadTexture(texturePathBuffer);
                            entity.hasTexture = (entity.texture.id != 0);
                        }
                    }
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Components", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Entities:");
        for (size_t i = 0; i < entities.size(); i++) {
            if (ImGui::Selectable(("Entity " + std::to_string(i)).c_str(), selectedEntity == (int)i)) {
                selectedEntity = i;
            }
        }

        if (ImGui::Button("Add Component")) {
            CreateEntity({ 400, 300 }, { 50, 50 }, BLUE, nullptr);
        }
    }
    ImGui::End();

    if (ImGui::Begin("File Manager", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::TextUnformatted("This is where you manage files.");
    }
    ImGui::End();

    rlImGuiEnd();
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
    EndDrawing();
}


void Engine::Render() {
    // Additional render logic if necessary
}

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
char TextureDir[128] = "C:/Users/barko/Documents/projects/xethium-engine/xethium engine/resources";

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
    Texture2D texture = LoadTexture(TextureDir);
    Texture2D textures = LoadTexture(Component::texturePath);
    Vector2 mousePos = GetMousePosition();

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

    bool inCollision = CheckCollisionPointRec(mousePos, { Component::entities[0].position.x, Component::entities[0].position.y, Component::entities[0].size.x, Component::entities[0].size.y });
    DrawTextureEx(textures, { 50, 50 }, 0.0f, 1.0f, WHITE);
    //cube.Render();
    Component::RenderEntities();
    grid.Render(cameraController.GetCamera());
    EndMode2D();

    // End rendering to the texture
    EndTextureMode();

    // Start a new ImGui frame
    rlImGuiBegin();  // Begin ImGui rendering with Raylib

    ImVec2 viewport_size = ImGui::GetMainViewport()->Size;

    // If the viewport size has changed, update the docking layout size
    static ImVec2 previous_size = ImVec2(0, 0);
    if (viewport_size.x != previous_size.x || viewport_size.y != previous_size.y) {
        // Update the docking space size to the new viewport size
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport_size);

        previous_size = viewport_size;
    }

#ifdef IMGUI_HAS_DOCK
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, NULL, ImGuiDockNodeFlags_PassthruCentralNode);
#endif


    //panel.EntityPropertyWindow();
    panel.SetupDocking();

    // Render ImGui windows
    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        rlImGuiImageRenderTextureFit(&(panel.ViewportTexture), true);
        ImVec2 viewportPos = ImGui::GetWindowPos();
        ImVec2 viewportSize = ImGui::GetWindowSize();

        // Convert screen mouse position to window-relative coordinates
        Vector2 localMousePos = { mousePos.x - viewportPos.x, mousePos.y - viewportPos.y };

        // Now you can check for collision within the ImGui window space
        bool inTextureCollision = CheckCollisionPointRec(localMousePos, { Component::entities[0].position.x, Component::entities[0].position.y, Component::entities[0].size.x, Component::entities[0].size.y });

        if (inTextureCollision) {
            std::cout << "Colided\n";
        }
    }
    ImGui::End();

    if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
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

            ImGui::Checkbox(" :Texture", &isTextured);
            if (isTextured) {
                ImGui::InputText(" :Dir", TextureDir, sizeof(TextureDir));


                if (texture.width == 0 || texture.height == 0) {
                    //printf("Failed to load texture!\n");
                }
                else {
                    //printf("Loaded!\n");
                    UnloadTexture(texture);
                }

                
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Components", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("Add Component")) {
            Component::CreateEntity({ 50, 50 }, { 100, 100 }, RED, Component::texturePath);
            //TextureDir = LoadTexture(Component::texturePath);
            component.PrintEntities();
        }
        
    }
    ImGui::End();


    if (ImGui::Begin("File Manager", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
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

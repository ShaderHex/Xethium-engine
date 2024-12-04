#include "Panel.h"
#include "imgui.h"
#include <iostream>
#include "imgui_impl_raylib.h"
#include "Engine.h"
#include "Component.h"

RenderTexture ViewportTexture;
bool IsOnViewport = true;

void Panel::Init() {
    ViewportTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    // Load custom font (Roboto.ttf)
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/roboto.ttf", 18.0f);

    if (!font) {
        std::cout << "Couldn't load the font!" << std::endl;
    } else {
        std::cout << "Font loaded successfully." << std::endl;
    }
    IM_ASSERT(font != nullptr);
    ImGui_ImplRaylib_BuildFontAtlas(); // Build the font atlas

    // Set the loaded font as the default font
    io.FontDefault = font;
}

void Panel::Render() {
    // Process events and start a new frame
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    // Simple window with custom font
    ImGui::Begin("Hello World!");
    ImGui::Text("This is using the custom font!");
    ImGui::End();

    // Render
    ImGui::Render();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
    EndDrawing();
}

void Panel::Update() {
    // Handle resizing if window dimensions change
    if (ViewportTexture.texture.width != GetScreenWidth() ||
        ViewportTexture.texture.height != GetScreenHeight()) {
        UnloadRenderTexture(ViewportTexture);
        ViewportTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }
}

//void Panel::EntityPropertyWindow() {
//    if (Component::selectedEntity >= 0 && Component::selectedEntity < Component::entityCount) {
//        Entity& entity = Component::entities[Component::selectedEntity];
//
//        ImGui::Begin("Entity Properties");
//
//        // Display and edit position
//        ImGui::DragFloat2("Position", (float*)&entity.position, 1.0f);
//
//        // Display and edit size
//        ImGui::DragFloat2("Size", (float*)&entity.size, 1.0f);
//
//        // Display and edit color
//        ImGui::ColorEdit3("Color", (float*)&entity.color);
//
//        ImGui::End();
//    }
//}


void Panel::Shutdown() {
    UnloadRenderTexture(ViewportTexture);
}

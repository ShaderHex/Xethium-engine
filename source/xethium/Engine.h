#pragma once
#include "raylib.h"
#include "Panel.h"
#include "Camera2D.h"
#include "GameObject.h"
#include "Grid.h"
#include "Component.h"

extern bool isTextured;

class Engine {
public:
    bool Init();
    void Run();
    void Renderer();
    void Shutdown();

private:
    Panel panel;
    Grid grid;
    Camera2DController cameraController;

    void Update();
    void Render();
};

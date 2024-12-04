#pragma once
#include "raylib.h"
#include "Panel.h"
#include "Camera2D.h"
#include "GameObject.h"
#include "Cube.h"
#include "Grid.h"
#include "Component.h"

class Engine {
public:
    Component component;
    
    bool Init();
    void Run();
    void Renderer();
    void Shutdown();

private:
    Panel panel;
    Cube cube;
    Grid grid;
    Camera2DController cameraController;

    void Update();
    void Render();
};

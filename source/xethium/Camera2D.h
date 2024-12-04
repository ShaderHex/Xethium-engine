#pragma once

#include "raylib.h"

class Camera2DController {
public:
    Camera2DController();     
    void Init();
    void Update();               
    Camera2D GetCamera() const;  
private:
    Camera2D camera;             
};

#pragma once
#include "raylib.h"
#include "imgui.h"
#include "GameObject.h"

class Panel : public GameObject {
public:
    //Panel();
    //~Panel();

    void Init();
    void Update() override;
    void Render() override;
    void SetupDocking();
    void Shutdown();
    RenderTexture2D ViewportTexture;

};

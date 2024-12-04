#pragma once
#include "raylib.h"
#include "imgui.h"
#include "GameObject.h"

extern float CubeX, CubeY;
extern float sCubeX, sCubeY;

class Cube : public GameObject {
public:
    void Update() override;
    void Render() override;
};
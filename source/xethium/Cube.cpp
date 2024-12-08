#include "raylib.h"
#include "imgui.h"
#include "GameObject.h"
#include "Cube.h"
#include <iostream>

float CubeX = 50.0f, CubeY = 50.0f;
float sCubeX = 100.0f, sCubeY = 100.0f;


void Cube::Render() {
    DrawRectangle(CubeX, CubeY, sCubeX, sCubeY, RED);
}
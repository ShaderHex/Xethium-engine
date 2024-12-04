#include "raylib.h"
#include "imgui.h"
#include "GameObject.h"
#include "Cube.h"
#include <iostream>

float CubeX = 50.0f, CubeY = 50.0f;
float sCubeX = 100.0f, sCubeY = 100.0f;

void Cube::Update() {
    if (IsKeyDown(KEY_A)) {
        CubeX -= 1;
        std::cout << "CUBE MOVING TO LEFT\n";
    }
    if (IsKeyDown(KEY_D)) {
        CubeX += 1;
        std::cout << "CUBE MOVING TO RIGHT\n";
    }
    if (IsKeyDown(KEY_W)) {
        CubeY += 1;
        std::cout << "CUBE MOVING TO UPWARD\n";
    }
    if (IsKeyDown(KEY_S)) {
        CubeY -= 1;
        std::cout << "CUBE MOVING TO DOWNWARD\n";
    }
}

void Cube::Render() {
    DrawRectangle(CubeX, CubeY, sCubeX, sCubeY, RED);
}
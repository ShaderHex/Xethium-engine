#include "Camera2D.h"
#include "raylib.h"
#include <iostream>

float cameraMoveSpeed = 0.0;
float cameraZoomSpeed = 0.0;

Camera2DController::Camera2DController() {
    camera.target = { 0.0f, 0.0f };    // Where the camera looks at
    camera.offset = { 00.0f, 00.0f };  // Camera position (centered on screen for 1200x800)
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;  // Default zoom level
}

void Camera2DController::Init() {
    cameraMoveSpeed = 500.0f;
    cameraZoomSpeed = 1.0f;
}

void Camera2DController::Update() {
    float deltaTime = GetFrameTime();

    // Movement with arrow keys
    if (IsKeyDown(KEY_RIGHT)) {
        camera.target.x += cameraMoveSpeed * deltaTime;
    }

    if (IsKeyDown(KEY_LEFT)) {
        camera.target.x -= cameraMoveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_UP)) {
        camera.target.y -= cameraMoveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN)) {
        camera.target.y += cameraMoveSpeed * deltaTime;
    }

    // Zoom handling
    if (IsKeyDown(KEY_Q)) {
        camera.zoom += cameraZoomSpeed * deltaTime;
    }  // Zoom in
    if (IsKeyDown(KEY_E)) {
        camera.zoom -= cameraZoomSpeed * deltaTime;
    }  // Zoom out

}

Camera2D Camera2DController::GetCamera() const {
    return camera;
}

#include <cmath>
#include "Grid.h"
#include "raylib.h"

void Grid::Update() {
    // Update grid properties if needed
}

void Grid::Render(const Camera2D& camera) {
    // Get the screen width and height
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    // Get the camera position and zoom factor
    Vector2 cameraPos = camera.target;
    float cameraZoom = camera.zoom;

    // Calculate the zoom factor (how much the camera is zoomed out)
    float zoomFactor = 1.0f / cameraZoom;

    // Draw the infinite X-axis (Red)
    // X-axis is drawn at y = 0 (crossing point)
    float xStart = cameraPos.x - screenWidth * zoomFactor; // Extend to the left
    float xEnd = cameraPos.x + screenWidth * zoomFactor;   // Extend to the right
    DrawLineV(Vector2{ xStart, screenHeight / 2.0f }, Vector2{ xEnd, screenHeight / 2.0f }, RED);  // Horizontal line at y = 0

    // Draw the infinite Y-axis (Blue)
    // Y-axis is drawn at x = 0 (crossing point)
    float yStart = cameraPos.y - screenHeight * zoomFactor; // Extend upwards
    float yEnd = cameraPos.y + screenHeight * zoomFactor;   // Extend downwards
    DrawLineV(Vector2{ screenWidth / 2.0f, yStart }, Vector2{ screenWidth / 2.0f, yEnd }, BLUE);  // Vertical line at x = 0
}

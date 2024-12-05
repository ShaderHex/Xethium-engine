#pragma once

#include "raylib.h"

struct Entity {
    Vector2 position = { 0, 0 };
    Vector2 size = { 0, 0 };
    Color color = { 255, 255, 255 };
    Texture2D texture = { 0 };
};

class Component {
public:
    static const int MAX_ENTITIES = 100;
    static Entity entities[MAX_ENTITIES];
    static int entityCount;
    static int selectedEntity;
    static const char* texturePath;

    static void CreateEntity(Vector2 position, Vector2 size, Color color, const char* texturePath);
    static void UpdateEntities(float deltaTime);
    static void RenderEntities();
    static void PrintEntities();
    static void Shutdown();
};

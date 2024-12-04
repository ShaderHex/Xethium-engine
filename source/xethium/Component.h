#pragma once

#include "raylib.h"

struct Entity {
    Vector2 position = { 0, 0 };
    Vector2 size = { 0, 0 };
    //unsigned int r;
    //unsigned int g;
    //unsigned int b;
    Color color = { 255, 255, 255 };
};

class Component {
public:
    static const int MAX_ENTITIES = 100;
    static Entity entities[MAX_ENTITIES];
    static int entityCount;
    static int selectedEntity;

    // Functions to create, update, and render entities
    static void CreateEntity(Vector2 position, Vector2 size, Color color);
    static void UpdateEntities(float deltaTime);
    static void RenderEntities();
};

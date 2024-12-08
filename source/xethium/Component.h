#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include <vector>
#include <string>

// Entity Structure
struct Entity {
    Vector2 position;
    Vector2 size;
    Color color;
    bool hasTexture;
    Texture2D texture;
    std::string texturePath; // To store the texture path for later use
};

// Functions
void CreateEntity(Vector2 position, Vector2 size, Color color, const char* texturePath = nullptr);
void RenderEntities();
void CleanupEntities();

// Global Variables
extern std::vector<Entity> entities;
extern int selectedEntity;

#endif // ENTITY_H

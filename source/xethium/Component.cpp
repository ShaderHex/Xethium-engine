#include "Component.h"

// Global Variables
std::vector<Entity> entities;
int selectedEntity = -1; // No entity selected by default

// Create a new entity
void CreateEntity(Vector2 position, Vector2 size, Color color, const char* texturePath) {
    Entity entity = { position, size, color, false, {}, texturePath ? texturePath : "" };
    if (texturePath) {
        entity.texture = LoadTexture(texturePath);
        entity.hasTexture = (entity.texture.id != 0);
    }
    entities.push_back(entity);
}

// Render all entities
void RenderEntities() {
    for (const Entity& entity : entities) {
        if (entity.hasTexture) {
            DrawTextureEx(entity.texture, entity.position, 0.0f, 1.0f, WHITE);
        }
        else {
            DrawRectangleV(entity.position, entity.size, entity.color);
        }
    }
}

// Clean up textures when exiting
void CleanupEntities() {
    for (auto& entity : entities) {
        if (entity.hasTexture) {
            UnloadTexture(entity.texture);
        }
    }
}

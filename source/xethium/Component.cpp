#include "Component.h"
#include <iostream>

Entity Component::entities[MAX_ENTITIES];
int Component::entityCount = 0;
int Component::selectedEntity = 0;
const char* Component::texturePath;

void Component::CreateEntity(Vector2 position, Vector2 size, Color color, const char* texturePath) {
    if (entityCount < MAX_ENTITIES) {
        entities[entityCount].position = position;
        entities[entityCount].size = size;
        entities[entityCount].color = color;

        if (texturePath != nullptr) {
            entities[entityCount].texture = LoadTexture(texturePath);

            if (entities[entityCount].texture.id == 0) {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
            }
        }

        entityCount++;
        printf("Entity Created: Position(%f, %f)\n", position.x, position.y);
    }
}


void Component::UpdateEntities(float deltaTime) {

}

void Component::RenderEntities() {
    for (int i = 0; i < entityCount; i++) {
        Entity& entity = entities[i];

        DrawRectangleV(entity.position, entity.size, entity.color);

        if (entity.texture.id != 0) {
            float scaleX = entity.size.x / (float)entity.texture.width;
            float scaleY = entity.size.y / (float)entity.texture.height;

            DrawTextureEx(entity.texture, entity.position, 0.0f, scaleX, WHITE);
            std::cout << "Rendering!\n";
        }
        else {
            std::cout << "Error when rendering image!\n";
        }
    }
}



void Component::PrintEntities() {
    for (int i = 0; i < Component::entityCount; ++i) {
        Entity& entity = Component::entities[i];
        std::cout << "Entity " << i << ":" << std::endl;
        std::cout << "  Position: (" << entity.position.x << ", " << entity.position.y << ")" << std::endl;
        std::cout << "  Size: (" << entity.size.x << ", " << entity.size.y << ")" << std::endl;
        std::cout << "  Color: (" << (int)entity.color.r << ", " << (int)entity.color.g << ", " << (int)entity.color.b << ")" << std::endl;
    }
}

void Component::Shutdown() {
    for (int i = 0; i < entityCount; i++) {
        UnloadTexture(entities[i].texture);
    }
}
#include "Component.h"
#include <iostream>

Entity Component::entities[MAX_ENTITIES];
int Component::entityCount = 0;
int Component::selectedEntity = 0;

void Component::CreateEntity(Vector2 position, Vector2 size, Color color) {
    if (entityCount < MAX_ENTITIES) {
        entities[entityCount].position = position;
        entities[entityCount].size = size;
        entities[entityCount].color = color;
        entityCount++;
        printf("Entity Created: Position(%f, %f)\n", position.x, position.y);
    }
}

void Component::UpdateEntities(float deltaTime) {

}

void Component::RenderEntities() {
    for (int i = 0; i < entityCount; i++) {
        //printf("Entity %d: Position(%f, %f) Size(%f, %f)\n", i, entities[i].position.x, entities[i].position.y, entities[i].size.x, entities[i].size.y);
        DrawRectangleV(entities[i].position, entities[i].size, entities[i].color);
    }
}


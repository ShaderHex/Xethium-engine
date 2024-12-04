#pragma once

#include "raylib.h"

class GameObject {
public:
    Vector3 position;

    GameObject() {
        position = { 0.0, 0.0, 0.0 };
    }

    virtual void Update() = 0;
    virtual void Render() = 0;
};
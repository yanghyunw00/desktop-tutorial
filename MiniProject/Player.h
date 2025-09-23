#pragma once

#include "raylib.h"
#include "Map.h"



class Player
{
public:
    Player();
    void Update(const Map& map);
    void Draw();

    void SetGridPosition(Vector2 gridPos);
    Vector2 GetGridPosition() const;
    Vector2 GetScreenPosition() const;

private:
    Vector2 gridPosition;

    Vector2 screenPosition;

    Vector2 targetPosition;

    Color playerColor;
    bool isMoving;
    float speed = 2.0f; 

    void UpdateScreenPosition();
};
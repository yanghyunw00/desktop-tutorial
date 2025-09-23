#include "Player.h"
#include <cmath>
#include <iostream>

extern const int screenWidth;
extern const int screenHeight;

Player::Player()
{

    gridPosition = { 0.0f, 0.0f };

    // screenPos 계산
    UpdateScreenPosition();

    targetPosition = screenPosition;

    playerColor = DARKBLUE;
    isMoving = false;
}

// gridPos -> screenPos
void Player::UpdateScreenPosition()
{
    screenPosition.x =  (gridPosition.x - gridPosition.y) * (ISO_TILE_WIDTH / 2.0f);
    screenPosition.y =  (gridPosition.x + gridPosition.y) * (ISO_TILE_HEIGHT / 2.0f);
}

void Player::SetGridPosition(Vector2 gridPos)
{
    gridPosition = gridPos;
    UpdateScreenPosition();
    targetPosition = screenPosition;
}

Vector2 Player::GetGridPosition() const
{
    return gridPosition;
}

Vector2 Player::GetScreenPosition() const
{
    return screenPosition;
}



void Player::Update(const Map& map)
{
    if (!isMoving)
    {
        Vector2 nextGridPos = gridPosition;

        if (IsKeyDown(KEY_W)) {
            nextGridPos.y--;
        }
        else if (IsKeyDown(KEY_S)) {
            nextGridPos.y++;
        }
        else if (IsKeyDown(KEY_A)) {
            nextGridPos.x--;
        }
        else if (IsKeyDown(KEY_D)) {
            nextGridPos.x++;
        }

        //  위치 변경이 있었는지 확인
        if (nextGridPos.x != gridPosition.x || nextGridPos.y != gridPosition.y)
        {
            // 맵 경계 & 타일 체크
            if (nextGridPos.x >= 0 && nextGridPos.x < MAP_WIDTH &&
                nextGridPos.y >= 0 && nextGridPos.y < MAP_HEIGHT &&
                map.IsWalkable(static_cast<int>(nextGridPos.y), static_cast<int>(nextGridPos.x)))
            {
                gridPosition = nextGridPos;

                targetPosition.x = (gridPosition.x - gridPosition.y) * (ISO_TILE_WIDTH / 2.0f);
                targetPosition.y = (gridPosition.x + gridPosition.y) * (ISO_TILE_HEIGHT / 2.0f);

                isMoving = true;
            }
        }
    }

    if (isMoving)
    {
        float dx = targetPosition.x - screenPosition.x;
        float dy = targetPosition.y - screenPosition.y;

        float distance = sqrtf(dx * dx + dy * dy);

        if (distance < speed)
        {
            screenPosition = targetPosition;
            isMoving = false;
        }
        else
        {
            //유닛벡터
            screenPosition.x += (dx / distance) * speed;
            screenPosition.y += (dy / distance) * speed;
        }
    }
}

void Player::Draw() 
{
    // 플레이어 타일중앙 위치
    float drawPosX = screenPosition.x + ISO_TILE_WIDTH / 2.0f;
    float drawPosY = screenPosition.y + ISO_TILE_HEIGHT / 2.0f;
    Color color = RED;
    DrawCircleV({ drawPosX, drawPosY }, 7, color);
}
#include "Player.h"
#include <cmath>
#include <iostream>

extern const int screenWidth;
extern const int screenHeight;

Player::Player()
{

    gridPosition = { 0.0f, 0.0f };

    // screenPos ���
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

        //  ��ġ ������ �־����� Ȯ��
        if (nextGridPos.x != gridPosition.x || nextGridPos.y != gridPosition.y)
        {
            // �� ��� & Ÿ�� üũ
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
            //���ֺ���
            screenPosition.x += (dx / distance) * speed;
            screenPosition.y += (dy / distance) * speed;
        }
    }
}

void Player::Draw() 
{
    // �÷��̾� Ÿ���߾� ��ġ
    float drawPosX = screenPosition.x + ISO_TILE_WIDTH / 2.0f;
    float drawPosY = screenPosition.y + ISO_TILE_HEIGHT / 2.0f;
    Color color = RED;
    DrawCircleV({ drawPosX, drawPosY }, 7, color);
}
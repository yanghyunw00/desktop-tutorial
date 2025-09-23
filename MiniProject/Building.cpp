#include "Building.h"
#include "raylib.h"
#include "Map.h"
#include "BuildingUi.h"


Building::Building(Texture2D texture, Vector2 gridPos)
    : buildingTexture(texture)
{
    mapOrigin = {0,0 };
    SetGridPosition(gridPos);
}

void Building::Update()
{

}


void Building::Draw(bool isSelected)
{
    Rectangle sourceRec = { 0,16 };
    sourceRec.width = 32; 
    sourceRec.height = 16; 

    Vector2 drawPos;
    drawPos.x = screenPosition.x;
    drawPos.y = screenPosition.y;
    
    Color alpha = isSelected ? Fade(GREEN, 0.7f) : WHITE;

    DrawTextureRec(buildingTexture, sourceRec, drawPos, alpha);
}

bool Building::WasClicked(Vector2 mouseWorldPos) const
{
    Rectangle rect = GetBuildingRect();

    return CheckCollisionPointRec(mouseWorldPos, rect);
}

Rectangle Building::GetBuildingRect() const
{
    Vector2 pos = { screenPosition.x, screenPosition.y };
    return { pos.x, pos.y, 32, 16 };
}

void Building::SetGridPosition(Vector2 gridPos)
{
    gridPosition = gridPos;
    UpdateScreenPosition();
}

Vector2 Building::GetGridPosition() const
{
    return gridPosition;
}

Vector2 Building::GetScreenPosition() const
{
    return screenPosition;
}

void Building::UpdateScreenPosition()
{
    screenPosition.x = mapOrigin.x + (gridPosition.x - gridPosition.y) * (ISO_TILE_WIDTH / 2.0f);
    screenPosition.y = mapOrigin.y + (gridPosition.x + gridPosition.y) * (ISO_TILE_HEIGHT / 2.0f);
}

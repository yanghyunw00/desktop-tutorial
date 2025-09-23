#include "BuildingUi.h"
#include <iostream>


BuildingUi::BuildingUi(Vector2 position_)
{
    position = position_;
	buildingUiTexture = (LoadTexture("assets/ui_map.png"));
}

BuildingUi::~BuildingUi()
{
	UnloadTexture(buildingUiTexture);
}

void BuildingUi::Update()
{
  
}

void BuildingUi::Draw ()
{
	Rectangle sourceRec = { 0.0f, 0.0f, (float)buildingUiTexture.width, (float)buildingUiTexture.height };

	DrawTextureRec(buildingUiTexture, sourceRec, position, WHITE);
}

Rectangle BuildingUi::GetBuildingUiRect() const
{
	Vector2 pos = { position.x, position.y };
	return { pos.x, pos.y, 256, 256 };
}

bool BuildingUi::UiWasClicked (Vector2 mouseWorldPos) const
{
	Rectangle rect = GetBuildingUiRect();

	return CheckCollisionPointRec (mouseWorldPos, rect);
}

Vector2 BuildingUi::GetScreenPosition () const
{
	return position;
}
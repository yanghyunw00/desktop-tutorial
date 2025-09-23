#pragma once
#include "raylib.h"
#include "Map.h"
#include "Player.h"

class Building
{
public:
	Building(Texture2D texture, Vector2 gridPos);
	void Update();
	void Draw(bool isSelected);
	Rectangle GetBuildingRect() const;
	bool WasClicked(Vector2 mouseWorldPos) const;
	void SetGridPosition(Vector2 gridPos);
	Vector2 GetGridPosition() const;
	Vector2 GetScreenPosition() const;

private:
	Texture2D buildingTexture; 
	Vector2 gridPosition;     
	Vector2 screenPosition;    

	Vector2 mapOrigin;
	void UpdateScreenPosition();


};

#pragma once
#include "raylib.h"
#include "Map.h"
#include "Player.h"

class BuildingUi
{
public:
	BuildingUi(Vector2 position);
	~BuildingUi();
	void Update ();
	void Draw ();
	Rectangle GetBuildingUiRect () const;
	bool UiWasClicked (Vector2 mouseWorldPos) const;
	Vector2 GetScreenPosition () const;

private:
	Texture2D buildingUiTexture;
	Vector2 position;

};

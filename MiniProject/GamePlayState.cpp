#include "GamePlayState.h"
#include "Player.h"
#include "raymath.h"
#include "raylib.h"
#include <iostream>

extern const int screenWidth;
extern const int screenHeight;



GamePlayState::GamePlayState()
	: tilemapTexture(LoadTexture("assets/tile_map.png")),

	map(tilemapTexture), ui( { 800,100 }),
	player()
{
	score = 0;
	camera.target = player.GetScreenPosition();
	camera.offset = { (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

}

GamePlayState::~GamePlayState()
{
	UnloadTexture(tilemapTexture);
}

void GamePlayState::Update()
{
	player.Update(map);

	camera.target = player.GetScreenPosition();

	float wheel = GetMouseWheelMove();

	if (wheel != 0)
	{
		camera.zoom += wheel * 0.1f;
		camera.zoom = Clamp(camera.zoom, 0.5f, 3.0f); 

	}


	Vector2 mouseScreenPos = GetMousePosition();
	Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
	Vector2 mouseGridPos = GetGridPosFromWorld(mouseWorldPos, map.GetOrigin());

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && selectedBuilding == nullptr && ui.UiWasClicked(mouseScreenPos))
	{
		isPlacingBuilding = true;
		buildings.emplace_back(tilemapTexture, mouseGridPos);
		selectedBuilding = &buildings.back();
	}
	//건물 선택
	else if (selectedBuilding != nullptr) 
	{
		selectedBuilding->SetGridPosition(mouseGridPos);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			selectedBuilding = nullptr;
			isPlacingBuilding = false;
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			if (isPlacingBuilding)
			{
				buildings.pop_back();
			}
			selectedBuilding = nullptr;
			isPlacingBuilding = false;
		}
	}
	//기존 건물 이동
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
	{
		for (int i = buildings.size() - 1; i >= 0; --i)
		{
			if (buildings[i].WasClicked(mouseWorldPos))
			{
				selectedBuilding = &buildings[i];
				isPlacingBuilding = false;
				break;
			}
		}
	}


	for (auto& building : buildings)
	{
		Vector2 playerGrid = player.GetGridPosition();
		Vector2 buildingGrid = building.GetGridPosition();
		std::vector<Vector2> path;
		path = FindPath(map, playerGrid, buildingGrid);
		shortPath.SetPath(path);
	}


	if (map.IsFinish(static_cast<int>(player.GetGridPosition().y), static_cast<int>(player.GetGridPosition().x)))
	{
		nextState = WIN;
	}

	if (IsKeyPressed(KEY_T))
	{
		nextState = TITLE;
	}
}

void GamePlayState::Draw()
{

	BeginMode2D(camera);
	map.Draw();
	shortPath.Draw(); 

	for (auto& building : buildings)
	{
		bool isSelected = (&building == selectedBuilding);
		building.Draw(isSelected);
	}

	player.Draw();
	EndMode2D();
	ui.Draw();

}

GameScreen GamePlayState::GetNextState()
{
	return nextState;
}

Vector2 GamePlayState::GetGridPosFromWorld(Vector2 worldPos, Vector2 mapOrigin)
{
	float mapX = worldPos.x - mapOrigin.x;
	float mapY = worldPos.y - mapOrigin.y;

	// screen -> grid
	float gridX = (mapX / (ISO_TILE_WIDTH / 2.0f) + mapY / (ISO_TILE_HEIGHT / 2.0f)) / 2.0f;
	float gridY = (mapY / (ISO_TILE_HEIGHT / 2.0f) - mapX / (ISO_TILE_WIDTH / 2.0f)) / 2.0f;

	return { floorf(gridX), floorf(gridY) };
}

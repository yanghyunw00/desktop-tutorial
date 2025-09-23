#pragma once
#include "GameState.h"
#include "Player.h"
#include "shortestPath.h"
#include "pathFinding.h"
#include "Building.h"
#include "BuildingUi.h"
#include "Map.h"

class GamePlayState : public GameState
{
private:
	Player player;
	Map map;
	BuildingUi ui;
	ShortPath shortPath;
	Camera2D camera = { 0 };

	GameScreen nextState = GAMEPLAY;

	Texture2D tilemapTexture;

	int score;

	std::vector<Building> buildings;
	Building* selectedBuilding = nullptr;
	bool isPlacingBuilding = false;

	~GamePlayState();
public:
	GamePlayState();
	void Update() override;
	void Draw() override;
	GameScreen GetNextState() override;
	int GetScore() { return score; }

	Vector2 GetGridPosFromWorld(Vector2 worldPos, Vector2 mapOrigin);



};
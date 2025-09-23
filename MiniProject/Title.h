#pragma once
#include "GameState.h"

class GameTitleState : public GameState
{
public:

	GameTitleState();
	void Update() override;
	void Draw() override;
	GameScreen GetNextState() override;
	~GameTitleState() override = default;

private:
	GameScreen nextState = TITLE;
	int selectedOption;

	void HandleInput();

};
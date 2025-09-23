#pragma once
#include "GameState.h"

class GameOverState : public GameState
{
public:

	void Update() override;
	void Draw() override;
	GameScreen GetNextState() override;

private:
	GameScreen nextState = GAMEOVER;
};
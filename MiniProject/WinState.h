#pragma once
#include "GameState.h"

class GameWinState : public GameState
{
public:
	GameWinState(int score) : finalScore(score) {}
	void Update() override;
	void Draw() override;
	GameScreen GetNextState() override;

private:
	int finalScore;

	GameScreen nextState = WIN;
};
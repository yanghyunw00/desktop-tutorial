#include "GameOver.h"



extern const int screenWidth;
extern const int screenHeight;

void GameOverState::Update()
{
	if (IsKeyPressed(KEY_R))
	{
		nextState = GAMEPLAY;
	}
	if (IsKeyPressed(KEY_BACKSPACE))
	{
		nextState = TITLE;
	}
}

void GameOverState::Draw()
{
	DrawText("GAME OVER!", screenWidth / 2 - 200, screenHeight / 2, 50, BLACK);
	DrawText("PRESS R TO RESTART", screenWidth / 2 - 200, screenHeight / 2 + 100, 30, GRAY);
	DrawText("PRESS BACKSPACE TO TITLE", screenWidth / 2 - 200, screenHeight / 2 + 200, 30, GRAY);

}

GameScreen GameOverState::GetNextState()
{
	return nextState;
}
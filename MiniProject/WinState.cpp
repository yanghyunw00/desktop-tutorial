#include "WinState.h"



extern const int screenWidth;
extern const int screenHeight;

void GameWinState::Update()
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

void GameWinState::Draw()
{

	const char* winText = "YOU WIN!!";
	const char* restartText = "PRESS R TO RESTART";
	const char* titleText = "PRESS BACKSPACE TO TITLE";


	int winFontSize = 70;
	int menuFontSize = 30;

	int winTextWidth = MeasureText(winText, winFontSize);
	int startTextWidth = MeasureText(restartText, menuFontSize);

	DrawText(winText, (GetScreenWidth() - winTextWidth) / 2, GetScreenHeight() / 2 - 250, winFontSize, BLACK);
	DrawText(restartText, (GetScreenWidth() - startTextWidth) / 2 - 30, GetScreenHeight() / 2 + 150, menuFontSize, GRAY);
	DrawText(titleText, (GetScreenWidth() - startTextWidth) / 2 - 70, GetScreenHeight() / 2 + 250, menuFontSize, GRAY);
	DrawText(TextFormat("SCORE: %i", finalScore), (GetScreenWidth() - winTextWidth) / 2, GetScreenHeight() / 2 - 100, winFontSize, BLACK);



}

GameScreen GameWinState::GetNextState()
{
	return nextState;
}
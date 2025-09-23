#include "Title.h"



extern const int screenWidth;
extern const int screenHeight;

GameTitleState::GameTitleState() : nextState(TITLE), selectedOption(0)
{
}

void GameTitleState::Update()
{
    HandleInput();
}

void GameTitleState::Draw()
{
    const char* titleText = "THE GAME";
    const char* startText = "GAME 1";

    const char* settingText = "SETTING";
    const char* exitText = "EXIT";
    const char* creatorText = "YANG HYUN WOO";

    int titleFontSize = 60;
    int menuFontSize = 30;

    int titleTextWidth = MeasureText(titleText, titleFontSize);
    int startTextWidth = MeasureText(startText, menuFontSize);
    int creatorTextWidth = MeasureText(creatorText, 20);

    //THE Game
    DrawText(titleText, (GetScreenWidth() - titleTextWidth) / 2, GetScreenHeight() / 2 - 200, titleFontSize, BLACK);

    //Start
    DrawText(startText, (GetScreenWidth() - startTextWidth) / 2 - 15, GetScreenHeight() / 2 - 100, menuFontSize, (selectedOption == 0) ? RED : BLACK);
    //Setting
    DrawText(settingText, (GetScreenWidth() - startTextWidth) / 2 - 30, GetScreenHeight() / 2 + 200, menuFontSize, (selectedOption == 1) ? RED : BLACK);

    //Exit
    DrawText(exitText, (GetScreenWidth() - startTextWidth) / 2, GetScreenHeight() / 2 + 300 , menuFontSize, (selectedOption == 2) ? RED : BLACK);


    //제작자
    DrawText(creatorText, (GetScreenWidth() - creatorTextWidth) / 2 + 440, GetScreenHeight() / 2 + 350, 20, DARKGRAY);

    //테두리
    DrawRectangleLinesEx({ 50, 50, (float)GetScreenWidth() - 100, (float)GetScreenHeight() - 100 }, 5, BLACK);
}

void GameTitleState::HandleInput()
{
    // 메뉴 이동
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedOption = (selectedOption + 1) % 3; // 시작, 옵션,종료
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedOption = (selectedOption - 1 + 3) % 3;
    }


    if (selectedOption == 0) //게임 1
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            nextState = GAMEPLAY;

        }
    }
    else if (selectedOption == 1) //설정
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            nextState = SETTING;

        }
    }
    else if (selectedOption == 2) //종료
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            nextState = EXIT;

        }
    }

}

GameScreen GameTitleState::GetNextState()
{
	return nextState;
}
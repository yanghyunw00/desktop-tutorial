#include "raylib.h"


#include "Player.h"


#include "Setting.h"
#include "Title.h"
#include "GameState.h"


#include "GamePlayState.h"

#include "GameOver.h"

#include "WinState.h"

#include <iostream>

extern const int screenWidth = 1200;
extern const int screenHeight = 900;


int main(void)
{

    InitWindow(screenWidth, screenHeight, "MINI PROJECT BY HYUNWOO YANG");
    SetTargetFPS(60);              
    GameState* currentState = new GameTitleState();
    GameScreen currentScreenEnum = TITLE;


    while (!WindowShouldClose())
    {
        currentState->Update();

        GameScreen nextScreenEnum = currentState->GetNextState();
        if (nextScreenEnum != currentScreenEnum)
        {
            GameState* nextState = nullptr;
            
            if (nextScreenEnum == TITLE)
            {
                nextState = new GameTitleState();
            }
            else if (nextScreenEnum == SETTING)
            {
                nextState = new GameSettingState();
            }
            else if (nextScreenEnum == EXIT)
            {
                return 0;
            }
//---------------------------------------------------------------//


            else if (nextScreenEnum == GAMEPLAY) 
            {
                nextState = new GamePlayState();
            }
            else if (nextScreenEnum == GAMEOVER) 
            {
                nextState = new GameOverState(); 
            }
            else if (nextScreenEnum == WIN)
            {
                int finalScore = static_cast<GamePlayState*>(currentState)->GetScore();
                nextState = new GameWinState(finalScore);
            }

           
            delete currentState;

            currentState = nextState;
            currentScreenEnum = nextScreenEnum;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        currentState->Draw();
        EndDrawing();
    }
    delete currentState;
    CloseWindow();      

    return 0;
}
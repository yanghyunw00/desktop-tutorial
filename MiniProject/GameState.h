#pragma once
#include "raylib.h"

typedef enum GameScreen {
    TITLE = 0,
    SETTING,
    GAMEPLAY,
    GAMEOVER,
    WIN ,
    EXIT
} GameScreen;

class GameState
{
public:
    virtual ~GameState() = default;

    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual GameScreen GetNextState() = 0;

private:

};

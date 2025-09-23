#pragma once
#include "GameState.h"
#include <vector>

class GameSettingState : public GameState
{
public:

	GameSettingState();
	void Update() override;
	void Draw() override;
	GameScreen GetNextState() override;
	~GameSettingState() override = default;

private:
	GameScreen nextState = SETTING;

    int selectedOption;
    
    // 소리 설정
    float volume;

    // 창 설정
    bool isFullscreen;
    std::vector<std::pair<int, int>> resolutions;
    int currentResolutionIndex;

    // Helper 함수들
    void HandleInput();
    void DrawOptions();

};
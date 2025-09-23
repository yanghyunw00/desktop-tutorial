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
    
    // �Ҹ� ����
    float volume;

    // â ����
    bool isFullscreen;
    std::vector<std::pair<int, int>> resolutions;
    int currentResolutionIndex;

    // Helper �Լ���
    void HandleInput();
    void DrawOptions();

};
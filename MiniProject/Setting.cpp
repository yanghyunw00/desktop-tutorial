#include "Setting.h"
#include <iostream>
#include <string>


extern const int screenWidth;
extern const int screenHeight;

GameSettingState::GameSettingState() : nextState(SETTING)
{
    resolutions.push_back({ 1200, 800 });
    resolutions.push_back({ 1920, 1080 });
    resolutions.push_back({ 1024, 768 });
    resolutions.push_back({ 1000, 1000 });


    int currentWidth = GetScreenWidth();
    int currentHeight = GetScreenHeight();
    currentResolutionIndex = 0;
    for (size_t i = 0; i < resolutions.size(); ++i)
    {
        if (resolutions[i].first == currentWidth && resolutions[i].second == currentHeight)
        {
            currentResolutionIndex = i;
            break;
        }
    }
}

void GameSettingState::Update()
{
    HandleInput();
}


void GameSettingState::HandleInput()
{
    // �޴� �̵�
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedOption = (selectedOption + 1) % 3; // �Ҹ�, â ����, �ػ� 3�� �ɼ�
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedOption = (selectedOption - 1 + 3) % 3;
    }

    // �ɼ� �� ����
    if (selectedOption == 0) // �Ҹ�
    {
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) && volume < 1.0f)
        {
            volume += 0.1f;
            SetMasterVolume(volume);
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) && volume > 0.0f)
        {
            volume -= 0.1f;
            SetMasterVolume(volume);
        }
    }
    else if (selectedOption == 1) // â ����
    {
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
        {
            isFullscreen = !isFullscreen;
            if (isFullscreen)
            {
                // ��üȭ������ ��ȯ
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            }
            else
            {
                // â���� ��ȯ
                ToggleFullscreen();
                SetWindowSize(resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second);
            }
        }
    }
    else if (selectedOption == 2) // �ػ�
    {
        if (IsKeyPressed(KEY_RIGHT))
        {
            currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();
            SetWindowSize(resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second);
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            currentResolutionIndex = (currentResolutionIndex - 1 + resolutions.size()) % resolutions.size();
            SetWindowSize(resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second);
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        nextState = TITLE;
    }
}

void GameSettingState::Draw()
{
    ClearBackground(RAYWHITE);
    DrawText("SETTINGS", GetScreenWidth() / 2 - MeasureText("SETTINGS", 50) / 2, 50, 50, BLACK);
    DrawOptions();
}

GameScreen GameSettingState::GetNextState()
{
    return nextState;
}

void GameSettingState::DrawOptions()
{
    const int startY = 200;
    const int spacing = 60;
    const int fontSize = 30;

    // �Ҹ�
    DrawText("Volume:", 100, startY, fontSize, (selectedOption == 0) ? RED : BLACK);
    DrawText(std::to_string(static_cast<int>(volume * 100)).c_str(), 400, startY, fontSize, (selectedOption == 0) ? RED : BLACK);

    // â ����
    DrawText("Fullscreen:", 100, startY + spacing, fontSize, (selectedOption == 1) ? RED : BLACK);
    DrawText(isFullscreen ? "ON" : "OFF", 400, startY + spacing, fontSize, (selectedOption == 1) ? RED : BLACK);

    // �ػ�
    std::string resolutionText = std::to_string(resolutions[currentResolutionIndex].first) + "x" + std::to_string(resolutions[currentResolutionIndex].second);
    DrawText("Resolution:", 100, startY + spacing * 2, fontSize, (selectedOption == 2) ? RED : BLACK);
    DrawText(resolutionText.c_str(), 400, startY + spacing * 2, fontSize, (selectedOption == 2) ? RED : BLACK);

    DrawText("Press BACKSPACE to go back to title", 50, GetScreenHeight() - 50, 20, DARKGRAY);
}
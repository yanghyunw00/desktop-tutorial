#include <raylib.h>
#include <iostream>
#include "GridMap.h"
#include "Mode7.h"
#include "CustomCell.h"

int main()
{
    const int screenWidth = 2000;
    const int screenHeight = 1500;
    InitWindow(screenWidth, screenHeight, "I.G.Y - With PathFinding");
    SetTargetFPS(60);
    srand(time(NULL));

    GridMap map;
    MouseInput mouse;

    // Initial generation
    map.ResetPerlinTexture();
    map.ResetGridMapArray();  // This will also initialize pathfinding
    map.ResetIsoGridMapArray();
    map.IsometricExportMap(); // generate initial map

    // Display instructions
    std::cout << "=== PathFinding Controls ===" << std::endl;
    std::cout << "Left Click: Set start point (green), then end point (blue)" << std::endl;
    std::cout << "R: Reset pathfinding selection" << std::endl;
    std::cout << "P: Toggle path visibility" << std::endl;
    std::cout << "Space: Generate new map" << std::endl;
    std::cout << "=============================" << std::endl;

    while (!WindowShouldClose())
    {
        // regenerate map with new Perlin noise
        if (IsKeyPressed(KEY_SPACE))
        {
            map.ResetPerlinTexture();
            map.ResetGridMapArray();  // This will reinitialize pathfinding
            map.ResetIsoGridMapArray();
            map.IsometricExportMap();
            std::cout << "New map generated!" << std::endl;
        }

        map.HandlePathFindingInput();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the isometric map with pathfinding
        map.IsometricDrawMap();

        // Draw UI information
        DrawFPS(1800, 10);

        // Draw instructions
        DrawText("Left Click: Set Start/End Points", 10, 10, 20, BLACK);
        DrawText("R: Reset | P: Toggle Path | Space: New Map", 10, 35, 20, BLACK);

        // Draw pathfinding status
        const char* status = "Click to set start point";
        if (map.IsWalkable(0, 0))
        { // Just checking if pathfinding is initialized
            static bool isSelectingStart = true;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (isSelectingStart)
                {
                    status = "Click to set end point";
                    isSelectingStart = false;
                }
                else
                {
                    status = "Path calculated! Press R to reset";
                    isSelectingStart = true;
                }
            }
            if (IsKeyPressed(KEY_R))
            {
                status = "Click to set start point";
                isSelectingStart = true;
            }
        }
        DrawText(status, 10, 60, 20, DARKGREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
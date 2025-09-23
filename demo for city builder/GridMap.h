#pragma once
#include <raylib.h>
#include <vector>
#include "GridCell.h"
#include "MouseInput.h"
#include "PathFinding.h"

#define GRID_SIZE_X 100
#define GRID_SIZE_Y 100
#define TILE_WIDTH 64
#define TILE_HEIGHT 32
#define TILE_WIDTH_HALF 32
#define TILE_HEIGHT_HALF 16

class GridMap
{
private:
    std::vector<GridCell> GridMapArray;
    std::vector<GridCell> IsoGridMapArray;

    int mapOriginX = 0;
    int mapOriginY = 0;

    Image PerlinNoiseImage;
    Texture2D PerlinNoiseTexture;
    Texture2D IsoMapTexture;
    Texture2D IsoGridTexture[4];
    Texture2D Max_Height;

    // PathFinding integration
    PathFinding* pathFinder;
    std::vector<Vector2> currentPath;
    Vector2 startPoint;
    Vector2 endPoint;
    bool showPath;
    bool isSelectingStart;
    bool isSelectingEnd;

public:
    GridMap();
    ~GridMap();

    // Original methods
    void DrawMap(int tileWidth, int tileHeight, MouseInput& mouse);
    void IsometricDrawMap();
    void ExportMapTexture();
    void IsometricExportMap();
    void ResetIsoGridMapArray();
    void ResetPerlinTexture();
    void ResetGridMapArray();
    void DrawPerlinTexture();
    const Texture2D* GetPerlinTexture() const;

    // PathFinding methods
    void InitializePathFinding();
    void HandlePathFindingInput();
    void FindPath(Vector2 start, Vector2 end);
    void FindPath(int startX, int startY, int endX, int endY);
    void ClearPath();
    void DrawCurrentPath();
    void DrawCurrentPathIsometric();
    void SetWalkable(int x, int y, bool walkable);
    bool IsWalkable(int x, int y);

    // Utility methods for pathfinding
    Vector2 ScreenToGrid(Vector2 screenPos, int tileWidth, int tileHeight);
    Vector2 ScreenToGridIsometric(Vector2 screenPos);
    Vector2 GridToScreen(Vector2 gridPos, int tileWidth, int tileHeight);
    Vector2 GridToScreenIsometric(Vector2 gridPos);
};
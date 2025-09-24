#pragma once
#include <raylib.h>
#include <vector>
#include "GridCell.h"
#include "MouseInput.h"
#include "PathFinding.h"

#define GRID_SIZE_X 32
#define GRID_SIZE_Y 32
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
    Texture PerlinNoiseTexture;
    Texture IsoMapTexture;
    Texture IsoGridTexture[4];
    Texture Max_Height;

    PathFinding* pathFinder;
    std::vector<Vector2> currentPath;
    bool showPath;

public:
    GridMap();
    ~GridMap();


    //Draw map function
    void DrawMap(int tileWidth, int tileHeight, MouseInput& mouse);
    void IsometricDrawMap();
    void ExportMapTexture();
    void IsometricExportMap();
    void ResetIsoGridMapArray();
    void ResetPerlinTexture();
    void ResetGridMapArray();
    void DrawPerlinTexture();
    const Texture* GetPerlinTexture() const;

    // PathFinding functions
    void InitializePathFinding();
    void HandlePathFindingInput();
    void FindPath(Vector2 start, Vector2 end);
    void FindPath(int startX, int startY, int endX, int endY);
    void ClearPath();
    void DrawCurrentPathIsometric(Color color);
    void SetWalkable(int x, int y, bool walkable);
    bool IsWalkable(int x, int y);

    Vector2 ScreenToGrid(Vector2 screenPos, int tileWidth, int tileHeight);
    Vector2 GridToScreen(Vector2 gridPos, int tileWidth, int tileHeight);

    Vector2 ScreenToGridIsometric(Vector2 screenPos);
    Vector2 GridToScreenIsometric(Vector2 gridPos);


    int isoOriginX = 0;
    int isoOriginY = 0;
};

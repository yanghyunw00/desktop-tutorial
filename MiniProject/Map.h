#pragma once
#include "raylib.h"
#include <vector>


typedef enum TileType {
	TILE_GROUND = 0,
	TILE_TREE = 1,
	TILE_WATER = 2,
    TILE_FINISH = 3,

} Tile;

constexpr int MAP_WIDTH = 40;
constexpr int MAP_HEIGHT = 40;


constexpr int ISO_TILE_WIDTH = 32;
constexpr int ISO_TILE_HEIGHT = 16;



class Map {
private:
    Texture2D tileTexture;

public:
    Map(Texture2D texture);
    ~Map();
    void Draw();
    std::vector<std::vector<TileType>> mapData;

    std::vector<std::vector<int>> heightData; 
    bool IsWalkable(int x, int y) const;
    bool IsFinish(int gridY, int gridX) const;
    Vector2 GetOrigin();


    Vector2 origin = {0,0};
};

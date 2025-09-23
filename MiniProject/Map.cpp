#include "Map.h"
#include <random>
#include "pathFinding.h"
#include <cmath>

extern const int screenWidth;
extern const int screenHeight;


Map::Map(Texture2D texture) : tileTexture(texture)
{
	std::vector<Vector2> path;

	do {
		mapData.assign(MAP_HEIGHT, std::vector<TileType>(MAP_WIDTH));

		std::random_device rd;
		std::mt19937 rand(rd());
		std::uniform_int_distribution<int> dist(0, 2);

		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				int randomTile = dist(rand);
				mapData[y][x] = static_cast<TileType>(randomTile);
			}
		}
		mapData[10][10] = static_cast<TileType>(TILE_FINISH);

		tileTexture = LoadTexture("assets/tile_map.png");


		path = FindPath(*this, { 0, 0 }, { MAP_WIDTH - 1, MAP_HEIGHT - 1 });

	} while (path.size() <= 2);
}

Map::~Map()
{
}


// screenWidth / 2.0f - ISO_TILE_WIDTH / 2.0f
void Map::Draw()
{
	origin.x = 0;
	origin.y = 0; 

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {

			// 아이소메트릭 공식
			Vector2 position;
			position.x = origin.x + (x - y) * (ISO_TILE_WIDTH / 2.0f);
			position.y = origin.y + (x + y) * (ISO_TILE_HEIGHT / 2.0f);

			Rectangle sourceRec;
			sourceRec.width = ISO_TILE_WIDTH;
			sourceRec.height = ISO_TILE_HEIGHT;
			sourceRec.y = 0;

			switch (mapData[y][x])
			{
			case TILE_GROUND:
				sourceRec.x = 0;
				break;
			case TILE_WATER:
				sourceRec.x = ISO_TILE_WIDTH * 2;
				break;
			case TILE_TREE:
				sourceRec.x = 0;
				break;
			case TILE_FINISH:
				sourceRec.x = ISO_TILE_WIDTH * 3;
				break;
			}

			DrawTextureRec(tileTexture, sourceRec, position, WHITE);

		}
	}

}


bool Map::IsWalkable(int gridY, int gridX) const
{
	if (gridY < 0 || gridY >= MAP_HEIGHT || gridX < 0 || gridX >= MAP_WIDTH)
		return false;

	return (mapData[gridY][gridX] != TILE_WATER);
}

bool Map::IsFinish(int gridY, int gridX) const  
{
	if (gridY < 0 || gridY >= MAP_HEIGHT || gridX < 0 || gridX >= MAP_WIDTH)
		return false;

	return (mapData[gridY][gridX] == TILE_FINISH);
}

Vector2 Map::GetOrigin()
{
	return origin;
}

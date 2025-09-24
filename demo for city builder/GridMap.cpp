
#include "GridMap.h"
#include <algorithm>
#include <vector>
#include <raymath.h>
#include <raylib.h>
#include <iostream>



GridMap::GridMap() : pathFinder(nullptr), showPath(false)
{
	ResetPerlinTexture();
	ResetGridMapArray();
	ResetIsoGridMapArray();

	IsoGridTexture[0] = LoadTexture("Resources/textures/IsoWater.png");
	IsoGridTexture[1] = LoadTexture("Resources/textures/IsoPlains.png");
	IsoGridTexture[2] = LoadTexture("Resources/textures/IsoMountains.png");
	IsoGridTexture[3] = LoadTexture("Resources/textures/IsoPeak.png");
	Max_Height = LoadTexture("Resources/textures/Max_Height.png");

	IsometricExportMap();
	IsoMapTexture = LoadTexture("IsoMap.png");
}


GridMap::~GridMap()
{
	if (pathFinder)
	{
		delete pathFinder;
	}

	for (int i = 0; i < 4; i++)
	{
		UnloadTexture(IsoGridTexture[i]);
	}
	UnloadTexture(Max_Height);
	UnloadTexture(IsoMapTexture);
	UnloadTexture(PerlinNoiseTexture);
}

void GridMap::DrawMap(int tileWidth, int tileHeight, MouseInput& mouse)
{

	mouse.MouseDrag();
	mouse.MouseScroll();

	Math::vec2 pan = mouse.GetPan();
	float zoom = mouse.GetZoom();

	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			double h = GridMapArray[y * GRID_SIZE_X + x].getHeight();
			unsigned char r = 0, g = 0, b = 0;

			if (h < 0.3) r = 0, g = 0, b = static_cast<unsigned char>(128 + 127 * (h / 0.3));
			else if (h < 0.6) r = 0, g = static_cast<unsigned char>(128 + 127 * ((h - 0.3) / 0.3)), b = 0;
			else if (h < 0.8) r = static_cast<unsigned char>(70 + 69 * ((h - 0.6) / 0.2)), g = 69, b = 19;
			else r = 255, g = 255, b = 255;

			Color color = { r, g, b, 255 };


			float normalX = (x * tileWidth) * zoom + pan.x;
			float normalY = (y * tileHeight) * zoom + pan.y;

			DrawRectangleV({ normalX, normalY }, { tileWidth * zoom, tileHeight * zoom }, color);
		}
	}

	HandlePathFindingInput();
}
//void GridMap::IsometricDrawMap(int tileWidth, int tileHeight, MouseInput& mouse)
//{
//
//	mouse.MouseDrag();
//	mouse.MouseScroll();
//
//	Math::vec2 pan = mouse.GetPan();
//	float zoom = mouse.GetZoom();
//
//	for (int y = 0; y < GRID_SIZE_Y; y++)
//	{
//		for (int x = 0; x < GRID_SIZE_X; x++)
//		{
//			double h = GridMapArray[y * GRID_SIZE_X + x].getHeight();
//
//			float fh = static_cast<float>(h);
//
//			unsigned char r = 0, g = 0, b = 0;
//
//			if (h < 0.3) r = 0, g = 0, b = static_cast<unsigned char>(128 + 127 * (h / 0.3));
//			else if (h < 0.6) r = 0, g = static_cast<unsigned char>(128 + 127 * ((h - 0.3) / 0.3)), b = 0;
//			else if (h < 0.8) r = static_cast<unsigned char>(70 + 69 * ((h - 0.6) / 0.2)), g = 69, b = 19;
//			else r = 255, g = 255, b = 255;
//
//			Color color = { r, g, b, 255 };
//
//
//				float isoX = (x - y) * (tileWidth / 2.0f) * zoom + pan.x;
//				float isoY = (x + y) * (tileHeight / 2.0f) * zoom + pan.y;
//				DrawRectangleV({ isoX , isoY },
//					{ tileWidth * zoom, tileHeight * zoom }, color);
//
//
//		}
//	}
//
//}
void GridMap::ExportMapTexture()
{

	float tileWidth = 10;
	float tileHeight = 10;

	RenderTexture2D target = LoadRenderTexture(GRID_SIZE_X * tileWidth, GRID_SIZE_Y * tileHeight);
	BeginTextureMode(target);

	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			double h = GridMapArray[y * GRID_SIZE_X + x].getHeight();
			unsigned char r = 0, g = 0, b = 0;

			if (h < 0.3) r = 0, g = 0, b = static_cast<unsigned char>(128 + 127 * (h / 0.3));
			else if (h < 0.6) r = 0, g = static_cast<unsigned char>(128 + 127 * ((h - 0.3) / 0.3)), b = 0;
			else if (h < 0.8) r = static_cast<unsigned char>(70 + 69 * ((h - 0.6) / 0.2)), g = 69, b = 19;
			else r = 255, g = 255, b = 255;

			Color color = { r, g, b, 255 };



			float normalX = (x * tileWidth);
			float normalY = (y * tileHeight);

			DrawRectangleV({ normalX, normalY }, { tileWidth, tileHeight }, color);

		}
	}
	EndTextureMode();
	Image image = LoadImageFromTexture(target.texture);
	ExportImage(image, "CreatedMap.png");
}

void GridMap::IsometricExportMap()
{

	int offsetupper = (int)IsoGridMapArray[0].getHeight();

	const int total_iso_width = TILE_WIDTH * GRID_SIZE_X;
	const int total_iso_height = TILE_HEIGHT * GRID_SIZE_Y + static_cast<int>(offsetupper) + 225;

	RenderTexture2D target = LoadRenderTexture(total_iso_width, total_iso_height);
	BeginTextureMode(target);
	ClearBackground(BLANK);
	
	int originX = (TILE_WIDTH_HALF * GRID_SIZE_X) - TILE_WIDTH / 2;
	int originY = static_cast<int>(offsetupper);


	// for path finding origin 
	isoOriginX = originX;
	isoOriginY = originY;


	int current_texture;
	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum)
	{
		for (int x = 0; x <= sum; ++x)
		{
			int y = sum - x;

			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y)
			{
				int screenX = originX + (x - y) * TILE_WIDTH_HALF;
				int screenY = originY + (x + y) * TILE_HEIGHT_HALF;

				int count = y * GRID_SIZE_X + x;
				GridCell currentCell = IsoGridMapArray[count];
				double height = currentCell.getHeight();

				current_texture = 3;
				if (height < 205) current_texture = 2;
				if (height < 150) current_texture = 1;
				if (height < 75) current_texture = 0;

				DrawTexture(IsoGridTexture[current_texture], screenX, screenY - static_cast<int>(height), WHITE);
				DrawTexture(Max_Height, screenX, screenY - static_cast<int>(height), WHITE);
			}
		}
	}

	EndTextureMode();
	Image image = LoadImageFromTexture(target.texture);
	ImageFlipVertical(&image);
	IsoMapTexture = LoadTextureFromImage(image);
	ExportImage(image, "IsoMap.png");

}

void GridMap::ResetIsoGridMapArray()
{

	IsoGridMapArray.resize(GRID_SIZE_X * GRID_SIZE_Y);

	int x = 0;
	int y = 0;
	int row = 1;
	int last_row = 0;
	int count = 0;

	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {

		for (int x = 0; x <= sum; ++x) {

			int y = sum - x;

			if (x < GRID_SIZE_X && y < GRID_SIZE_Y) {

				int count = y * GRID_SIZE_X + x; // or however you want to index

				unsigned char Height = GetImageColor(PerlinNoiseImage, x, y).r;

				IsoGridMapArray[count] = GridCell(Height);

			}

		}

	}

	InitializePathFinding();

	//for (int i = 0; i < IsoGridMapArray.size(); i++)
	//{
	//	std::cout << "(" << i << ") : " << IsoGridMapArray.at(i).getHeight() << " , ";

	//	if (i % 10 == 0 && i != 0)
	//		std::cout << std::endl;
	//}
	//std::cout << "//////////////////////DONE//////////////////////////////////" << std::endl;

}

void GridMap::IsometricDrawMap()
{
	int originX = isoOriginX;
	int originY = isoOriginY;

	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum)
	{
		for (int x = 0; x <= sum; ++x)
		{
			int y = sum - x;

			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y)
			{
				int count = y * GRID_SIZE_X + x;
				GridCell currentCell = IsoGridMapArray[count];
				double height = currentCell.getHeight();

				int current_texture;
				switch (currentCell.getType())
				{
				case TerrainType::Water:    current_texture = 0; break;
				case TerrainType::Plains:   current_texture = 1; break;
				case TerrainType::Mountain: current_texture = 2; break;
				case TerrainType::Snow:     current_texture = 3; break;
				default:                    current_texture = 1; break;
				}

				int screenX = originX + (x - y) * TILE_WIDTH_HALF;
				int screenY = originY + (x + y) * TILE_HEIGHT_HALF;

				DrawTexture(IsoGridTexture[current_texture], screenX, screenY - static_cast<int>(height), WHITE);
				DrawTexture(Max_Height, screenX, screenY - static_cast<int>(height), WHITE);
			}
		}
	}

	DrawCurrentPathIsometric(RED);
}

void GridMap::ResetPerlinTexture()
{
	int seedX = GetRandomValue(0, 10000);
	int seedY = GetRandomValue(0, 10000);
	std::cout << seedX << " ," << seedY << std::endl;
	PerlinNoiseImage = GenImagePerlinNoise(GRID_SIZE_X, GRID_SIZE_Y, seedX, seedY, 1.0f);
	PerlinNoiseTexture = LoadTextureFromImage(PerlinNoiseImage);


	ExportImage(PerlinNoiseImage, "PerlinNoise.png");

}

void GridMap::ResetGridMapArray()
{


	GridMapArray.resize(GRID_SIZE_X * GRID_SIZE_Y);

	for (int y = 0; y < GRID_SIZE_Y; y++)
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			unsigned char Height = GetImageColor(PerlinNoiseImage, y, x).r;
			//double normalizedHeightValue = (double)Height / 255.0;
			GridMapArray[y * GRID_SIZE_X + x] = GridCell{ (double)Height }; // initialize the GridMap Array by calling the constructor of the gridcell with random but coherent values. 
		}

	//for (int i = 0; i < GridMapArray.size(); i++)
	//{
	//	std::cout << "(" << i << ") : " << GridMapArray.at(i).getHeight() << " , ";

	//	if (i % 10 == 0 && i!= 0)
	//		std::cout << std::endl;
	//}
	//std::cout << "DONE" << std::endl;

}

void GridMap::DrawPerlinTexture()
{
	// get mouse scroll to scale the texture
	static float scale = 1.0f;
	float scroll = GetMouseWheelMove(); // +1 or -1 per scroll tick
	scale += scroll * 0.1f;             // adjust scaling speed
	if (scale < 0.1f) scale = 0.1f;     // minimum scale
	if (scale > 10.0f) scale = 10.0f;   // maximum scale

	// original texture size
	int texWidth = PerlinNoiseTexture.width;
	int texHeight = PerlinNoiseTexture.height;

	// calculate destination rectangle centered on screen
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	Rectangle destRec;
	destRec.width = texWidth * scale;
	destRec.height = texHeight * scale;
	destRec.x = screenWidth / 2.0f - destRec.width / 2.0f;
	destRec.y = screenHeight / 2.0f - destRec.height / 2.0f;

	Rectangle sourceRec = { 0, 0, (float)texWidth, (float)texHeight };
	Vector2 origin = { 0, 0 }; // top-left origin

	DrawTexturePro(PerlinNoiseTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}

const Texture2D* GridMap::GetPerlinTexture() const// so we dont copy the texture
{
	return &PerlinNoiseTexture;
}


/*-------------------------For Path Finding--------------------------------------*/


void GridMap::InitializePathFinding()
{
	if (pathFinder)
	{
		delete pathFinder;
	}

	pathFinder = new PathFinding(GRID_SIZE_X, GRID_SIZE_Y, 50.0f);

	std::vector<double> heights;
	std::vector<float> costs;

	heights.reserve(GridMapArray.size());

	for (const auto& cell : GridMapArray)
	{
		heights.push_back(cell.getHeight());
		costs.push_back(cell.getMovementCost()); 
	}

	pathFinder->SetHeightMap(heights);
	pathFinder->SetCostMap(costs);
	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			const auto& currentCell = IsoGridMapArray[y * GRID_SIZE_X + x];
			bool walkable = currentCell.isWalkable();
			pathFinder->SetWalkable(x, y, walkable);

			//인접 셀의 높이 차이 체크로 변경해야힘 
			// 낮은 곳 -> 높은 곳 X
			// 높은 곳 - > 낮은 곳 X
		}
	}
}

void GridMap::HandlePathFindingInput()
{

	ClearPath();
	std::cout << "Reset pathfinding" << std::endl;
}

void GridMap::FindPath(Vector2 start, Vector2 end)
{
	if (!pathFinder) return;

	currentPath = pathFinder->FindPath(start, end);
	showPath = true;

	std::cout << "Path found with " << currentPath.size() << " nodes" << std::endl;
}

void GridMap::FindPath(int startX, int startY, int endX, int endY)
{
	FindPath({ static_cast<float>(startX), static_cast<float>(startY) },
		{ static_cast<float>(endX), static_cast<float>(endY) });
}

void GridMap::ClearPath()
{
	currentPath.clear();
	showPath = false;
}

void GridMap::DrawCurrentPathIsometric(Color color)
{
	if (!pathFinder || currentPath.empty())
	{
		return;
	}


	pathFinder->DrawPathIsometric(currentPath, TILE_WIDTH, TILE_HEIGHT, isoOriginX, isoOriginY, color);
}

void GridMap::SetWalkable(int x, int y, bool walkable)
{
	if (pathFinder)
	{
		pathFinder->SetWalkable(x, y, walkable);
	}
}

bool GridMap::IsWalkable(int x, int y)
{
	if (pathFinder)
	{
		return pathFinder->IsPositionWalkable(x, y);
	}
	return false;
}

Vector2 GridMap::ScreenToGrid(Vector2 screenPos, int tileWidth, int tileHeight)
{
	return { screenPos.x / tileWidth, screenPos.y / tileHeight };
}

Vector2 GridMap::ScreenToGridIsometric(Vector2 screenPos)
{

	/*
float isoX = (screenPos.x / TILE_WIDTH_HALF + screenPos.y / TILE_HEIGHT_HALF) / 2.0f;
float isoY = (screenPos.y / TILE_HEIGHT_HALF - screenPos.x / TILE_WIDTH_HALF) / 2.0f;
return { isoX, isoY };
	*/
	float localX = screenPos.x - (float)isoOriginX;
	float localY = screenPos.y - (float)isoOriginY;

	float approxX = (localX / (float)TILE_WIDTH_HALF + localY / (float)TILE_HEIGHT_HALF) * 0.5f;
	float approxY = (localY / (float)TILE_HEIGHT_HALF - localX / (float)TILE_WIDTH_HALF) * 0.5f;

	int cx = static_cast<int>(std::floor(approxX));
	int cy = static_cast<int>(std::floor(approxY));

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			int tx = cx + dx;
			int ty = cy + dy;

			if (tx < 0 || tx >= GRID_SIZE_X || ty < 0 || ty >= GRID_SIZE_Y) continue;

			double tileHeight = IsoGridMapArray[ty * GRID_SIZE_X + tx].getHeight();
			int tileScreenX = isoOriginX + (tx - ty) * TILE_WIDTH_HALF;
			int tileScreenY = isoOriginY + (tx + ty) * TILE_HEIGHT_HALF - static_cast<int>(tileHeight);

			float relX = screenPos.x - (float)tileScreenX;
			float relY = screenPos.y - (float)tileScreenY;

			float nx = fabs(relX) / (float)TILE_WIDTH_HALF;
			float ny = fabs(relY) / (float)TILE_HEIGHT_HALF;

			if (nx + ny <= 1.0f)
			{
				return { static_cast<float>(tx), static_cast<float>(ty) };
			}
		}
	}

	if (cx >= 0 && cx < GRID_SIZE_X && cy >= 0 && cy < GRID_SIZE_Y)
		return { static_cast<float>(cx), static_cast<float>(cy) };

	return { -1.0f, -1.0f };

}

Vector2 GridMap::GridToScreen(Vector2 gridPos, int tileWidth, int tileHeight)
{
	return { gridPos.x * tileWidth, gridPos.y * tileHeight };
}

Vector2 GridMap::GridToScreenIsometric(Vector2 gridPos)
{
	float isoX = (gridPos.x - gridPos.y) * TILE_WIDTH_HALF;
	float isoY = (gridPos.x + gridPos.y) * TILE_HEIGHT_HALF;
	return { isoX, isoY };
}


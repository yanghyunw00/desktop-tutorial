
#include "GridMap.h"
#include <algorithm>
#include <vector>
#include <raymath.h>
#include <raylib.h>
#include <iostream>



GridMap::GridMap() : pathFinder(nullptr), showPath(false), isSelectingStart(true), isSelectingEnd(false)
{
	startPoint = { -1, -1 };
	endPoint = { -1, -1 };
}

GridMap::~GridMap()
{
	if (pathFinder)
	{
		delete pathFinder;
	}
}

void GridMap::InitializePathFinding()
{
	if (pathFinder)
	{
		delete pathFinder;
	}

	pathFinder = new PathFinding(GRID_SIZE_X, GRID_SIZE_Y, 50.0f);

	// Convert GridMapArray heights to vector<double>
	std::vector<double> heights;
	heights.reserve(GridMapArray.size());

	for (const auto& cell : GridMapArray)
	{
		heights.push_back(cell.getHeight());
	}

	pathFinder->SetHeightMap(heights);

	// Set walkability based on height (water areas might not be walkable)
	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			double height = GridMapArray[y * GRID_SIZE_X + x].getHeight();
			// Make water areas (height < 75) unwalkable, or adjust as needed
			bool walkable = height >= 30.0; // Adjust threshold as needed
			pathFinder->SetWalkable(x, y, walkable);
		}
	}
}


void GridMap::HandlePathFindingInput()
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 mousePos = GetMousePosition();
		Vector2 gridPos = ScreenToGrid(mousePos, 10, 10); // Adjust tile size as needed

		if (gridPos.x >= 0 && gridPos.x < GRID_SIZE_X &&
			gridPos.y >= 0 && gridPos.y < GRID_SIZE_Y)
		{

			if (isSelectingStart)
			{
				startPoint = gridPos;
				isSelectingStart = false;
				isSelectingEnd = true;
				std::cout << "Start point set at: " << gridPos.x << ", " << gridPos.y << std::endl;
			}
			else if (isSelectingEnd)
			{
				endPoint = gridPos;
				isSelectingEnd = false;
				FindPath(startPoint, endPoint);
				std::cout << "End point set at: " << gridPos.x << ", " << gridPos.y << std::endl;
				std::cout << "Path calculated!" << std::endl;
			}
		}
	}

	// Reset selection with R key
	if (IsKeyPressed(KEY_R))
	{
		ClearPath();
		isSelectingStart = true;
		isSelectingEnd = false;
		startPoint = { -1, -1 };
		endPoint = { -1, -1 };
		std::cout << "Reset pathfinding selection" << std::endl;
	}

	// Toggle path visibility with P key
	if (IsKeyPressed(KEY_P))
	{
		showPath = !showPath;
		std::cout << "Path visibility: " << (showPath ? "ON" : "OFF") << std::endl;
	}
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

void GridMap::DrawCurrentPath()
{
	if (!showPath || currentPath.empty() || !pathFinder) return;

	pathFinder->DrawPath(currentPath, 10, 10, RED);

	// Draw start and end points
	if (startPoint.x >= 0 && startPoint.y >= 0)
	{
		DrawRectangle(static_cast<int>(startPoint.x * 10),
			static_cast<int>(startPoint.y * 10), 10, 10, GREEN);
	}

	if (endPoint.x >= 0 && endPoint.y >= 0)
	{
		DrawRectangle(static_cast<int>(endPoint.x * 10),
			static_cast<int>(endPoint.y * 10), 10, 10, BLUE);
	}
}

void GridMap::DrawCurrentPathIsometric()
{
	if (!showPath || currentPath.empty() || !pathFinder) return;

	pathFinder->DrawPathIsometric(currentPath, TILE_WIDTH, TILE_HEIGHT, RED);

	// Draw start and end points in isometric view
	if (startPoint.x >= 0 && startPoint.y >= 0)
	{
		int isoX = (static_cast<int>(startPoint.x) - static_cast<int>(startPoint.y)) * TILE_WIDTH_HALF;
		int isoY = (static_cast<int>(startPoint.x) + static_cast<int>(startPoint.y)) * TILE_HEIGHT_HALF;
		DrawCircle(isoX, isoY, 8, GREEN);
	}

	if (endPoint.x >= 0 && endPoint.y >= 0)
	{
		int isoX = (static_cast<int>(endPoint.x) - static_cast<int>(endPoint.y)) * TILE_WIDTH_HALF;
		int isoY = (static_cast<int>(endPoint.x) + static_cast<int>(endPoint.y)) * TILE_HEIGHT_HALF;
		DrawCircle(isoX, isoY, 8, BLUE);
	}
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
	// Convert screen coordinates to isometric grid coordinates
	float isoX = (screenPos.x / TILE_WIDTH_HALF + screenPos.y / TILE_HEIGHT_HALF) / 2.0f;
	float isoY = (screenPos.y / TILE_HEIGHT_HALF - screenPos.x / TILE_WIDTH_HALF) / 2.0f;
	return { isoX, isoY };
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

	DrawCurrentPath();

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
		
	RenderTexture2D target = LoadRenderTexture(GRID_SIZE_X* tileWidth, GRID_SIZE_Y* tileHeight);
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

				DrawRectangleV({ normalX, normalY }, { tileWidth, tileHeight}, color);

		}
	}
	EndTextureMode();
	Image image = LoadImageFromTexture(target.texture);
	
	ExportImage(image, "CreatedMap.png");
}

void GridMap::IsometricExportMap()
{

	int offsetupper = (int)IsoGridMapArray[0].getHeight();

	RenderTexture2D target = LoadRenderTexture(TILE_WIDTH* GRID_SIZE_X,TILE_HEIGHT*GRID_SIZE_Y+ offsetupper + 225);
	BeginTextureMode(target);
	
	int originX = ((TILE_WIDTH/2)*GRID_SIZE_X) - TILE_WIDTH /2;
	int originY = offsetupper;
	int current_texture;

	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {
		for (int x = 0; x <= sum; ++x) {
			
			int y = sum - x;


			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y) {


				int screenX = originX + (x - y) * TILE_WIDTH_HALF;
				int screenY = originY + (x + y) * TILE_HEIGHT_HALF;

				

				int count = y * GRID_SIZE_X + x;
				GridCell currentCell = IsoGridMapArray[count];
				double height = currentCell.getHeight();

				current_texture = 3;
				if(height < 205) current_texture = 2;
				if (height < 150) current_texture = 1;
				if (height < 75) current_texture = 0;

				DrawTexture(IsoGridTexture[current_texture], screenX, screenY - height, WHITE);
				DrawTexture(Max_Height, screenX, screenY- height, WHITE);
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

				IsoGridMapArray[count] = GridCell{ (double)Height };

			}

		}

	}
	//for (int i = 0; i < IsoGridMapArray.size(); i++)
	//{
	//	std::cout << "(" << i << ") : " << IsoGridMapArray.at(i).getHeight() << " , ";

	//	if (i % 10 == 0 && i != 0)
	//		std::cout << std::endl;
	//}
	//std::cout << "//////////////////////DONE//////////////////////////////////" << std::endl;

	InitializePathFinding();

}

void GridMap::IsometricDrawMap()
{
	if (IsoMapTexture.id != 0)
	{
		DrawTexture(IsoMapTexture, 0, 0, WHITE);
	}
	DrawCurrentPathIsometric();
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

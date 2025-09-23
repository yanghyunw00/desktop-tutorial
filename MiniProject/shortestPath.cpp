#include "shortestPath.h"


void ShortPath::SetPath(const std::vector<Vector2>& newPath) {
	path = newPath;

}

void ShortPath::Draw() const
{
	if (path.size() < 2) return;

	const float offsetX = 16.0f; 
	const float offsetY = 8.0f; 

	for (size_t i = 0; i < path.size() - 1; i++)
	{
		Vector2 startPos = { path[i].x + offsetX, path[i].y + offsetY };
		Vector2 endPos = { path[i + 1].x + offsetX, path[i + 1].y + offsetY };

		DrawLineEx(startPos, endPos, 5, RED);
		DrawCircleV(startPos, 5, RED);
	}

	Vector2 lastPos = { path.back().x + offsetX, path.back().y + offsetY };
	DrawCircleV(lastPos, 5, RED);
}

void ShortPath::Clear() {
	path.clear();
}


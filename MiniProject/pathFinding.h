#pragma once
#include "Map.h"
#include "raylib.h"
#include <vector>

//a* 알고리즘
std::vector<Vector2> FindPath(const Map& map, Vector2 startPos, Vector2 goalPos);


//스폰시 장애물 위 스폰 방지 및 격자 맟추기
Vector2 FindNearestGround(const Map& map, Vector2 inputPos);


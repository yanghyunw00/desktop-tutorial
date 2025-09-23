#pragma once
#include "Map.h"
#include "raylib.h"
#include <vector>

//a* �˰���
std::vector<Vector2> FindPath(const Map& map, Vector2 startPos, Vector2 goalPos);


//������ ��ֹ� �� ���� ���� �� ���� ���߱�
Vector2 FindNearestGround(const Map& map, Vector2 inputPos);


#include "pathFinding.h"
#include "Map.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cmath>

extern const int screenWidth;
extern const int screenHeight;

struct Node {
    int x, y;
    float g, h;
    // f = g(출발지부터 현재 노드, 지금까지 이동한 비용) + h(노드부터 목표까지 예상)
    // f = g 라면 다익스트라와 동일함
    Node* parent;
};

struct NodeCompare {
    bool operator()(Node* a, Node* b) {
        return (a->g + a->h) > (b->g + b->h);
    }
};

//맨해튼 거리공식
static float Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// a* 알고리즘
std::vector<Vector2> FindPath(const Map& map, Vector2 startGrid, Vector2 goalGrid)
{
    int startX = static_cast<int>(startGrid.x);
    int startY = static_cast<int>(startGrid.y);
    int goalX = static_cast<int>(goalGrid.x);
    int goalY = static_cast<int>(goalGrid.y);

    auto key = [&](int x, int y) { return y * MAP_WIDTH + x; };

    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> openList;
    std::unordered_map<int, Node*> allNodes;
    std::unordered_map<int, bool> closedList;

    Node* startNode = new Node{ startX, startY, 0, Heuristic(startX, startY, goalX, goalY), nullptr };
    openList.push(startNode);
    allNodes[key(startX, startY)] = startNode;

    Node* goalNode = nullptr;

    int dir[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!openList.empty())
    {
        Node* current = openList.top();
        openList.pop();

        if (current->x == goalX && current->y == goalY)
        {
            goalNode = current;
            break;
        }

        closedList[key(current->x, current->y)] = true;

        for (auto& d : dir)
        {
            int nx = current->x + d[0];
            int ny = current->y + d[1];

            if (nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) continue;
            if (!map.IsWalkable(ny, nx)) continue;
            if (closedList.count(key(nx, ny))) continue;

            float gCost = current->g + 1;
            float hCost = Heuristic(nx, ny, goalX, goalY);

            if (allNodes.find(key(nx, ny)) == allNodes.end() || gCost < allNodes[key(nx, ny)]->g)
            {
                Node* neighbor = new Node{ nx, ny, gCost, hCost, current };
                openList.push(neighbor);
                allNodes[key(nx, ny)] = neighbor;
            }
        }
    }

    std::vector<Vector2> path;
    if (goalNode)
    {
        Vector2 origin = { 0.0f, 0.0f }; 

        for (Node* n = goalNode; n != nullptr; n = n->parent)
        {
            Vector2 position;
            position.x = origin.x + (n->x - n->y) * (ISO_TILE_WIDTH / 2.0f);
            position.y = origin.y + (n->x + n->y) * (ISO_TILE_HEIGHT / 2.0f);
            path.push_back(position);
        }
        std::reverse(path.begin(), path.end());
    }

    for (auto& p : allNodes) delete p.second;

    return path;
}


//BFS 탐색
//Vector2 FindNearestGround(const Map& map, Vector2 inputPos) 
//{
//    int startX = (int)(inputPos.x / TILE_SIZE_W);
//    int startY = (int)(inputPos.y / TILE_SIZE_H);
//
//    if (map.IsWalkable(startY, startX))
//    {
//        return { startX * (float)TILE_SIZE_W, startY * (float)TILE_SIZE_H };
//    }
//
//
//    //BFS 알고리즘
//    std::queue<std::pair<int, int>> q;
//    std::unordered_map<int, bool> visited; //방문한 곳을 기록
//
//    //2차원을 1차원으로 나열
//    auto key = [&](int x, int y) { return y * MAP_WIDTH + x; };
//
//    q.push({ startX, startY });
//    visited[key(startX, startY)] = true;
//
//    //8방향의 타일을 확인
//    int dir[8][2] = {
//    {1, 0},   //  오른쪽
//    {-1, 0},  //  왼쪽
//    {0, 1},   //  아래
//    {0, -1},  //  위
//    {1, 1},   //  오른쪽 아래
//    {1, -1},  //  오른쪽 위
//    {-1, 1},  //  왼쪽 아래
//    {-1, -1}  //  왼쪽 위
//    };
//
//    while (!q.empty()) {
//        //젤 앞에를 꺼내서 확인
//        auto [cx, cy] = q.front();
//        q.pop();
//
//        for (auto& d : dir) 
//        {
//            //4방향 순회
//            int nx = cx + d[0];
//            int ny = cy + d[1];
//
//            //맵 범위 밖, 방문한 노드 continue;
//            if (nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT) continue;
//            if (visited[key(nx, ny)]) continue;
//
//            visited[key(nx, ny)] = true;
//
//            if (map.IsWalkable(ny, nx))
//            {
//                return { nx * (float)TILE_SIZE_W, ny * (float)TILE_SIZE_H };
//            }
//
//            q.push({ nx, ny });
//        }
//    }
//
//    //없다면 그냥 반환(4방향이 막혔을 경우
//    return { startX * (float)TILE_SIZE_W, startY * (float)TILE_SIZE_H };
//}
//




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
    // f = g(��������� ���� ���, ���ݱ��� �̵��� ���) + h(������ ��ǥ���� ����)
    // f = g ��� ���ͽ�Ʈ��� ������
    Node* parent;
};

struct NodeCompare {
    bool operator()(Node* a, Node* b) {
        return (a->g + a->h) > (b->g + b->h);
    }
};

//����ư �Ÿ�����
static float Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// a* �˰���
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


//BFS Ž��
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
//    //BFS �˰���
//    std::queue<std::pair<int, int>> q;
//    std::unordered_map<int, bool> visited; //�湮�� ���� ���
//
//    //2������ 1�������� ����
//    auto key = [&](int x, int y) { return y * MAP_WIDTH + x; };
//
//    q.push({ startX, startY });
//    visited[key(startX, startY)] = true;
//
//    //8������ Ÿ���� Ȯ��
//    int dir[8][2] = {
//    {1, 0},   //  ������
//    {-1, 0},  //  ����
//    {0, 1},   //  �Ʒ�
//    {0, -1},  //  ��
//    {1, 1},   //  ������ �Ʒ�
//    {1, -1},  //  ������ ��
//    {-1, 1},  //  ���� �Ʒ�
//    {-1, -1}  //  ���� ��
//    };
//
//    while (!q.empty()) {
//        //�� �տ��� ������ Ȯ��
//        auto [cx, cy] = q.front();
//        q.pop();
//
//        for (auto& d : dir) 
//        {
//            //4���� ��ȸ
//            int nx = cx + d[0];
//            int ny = cy + d[1];
//
//            //�� ���� ��, �湮�� ��� continue;
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
//    //���ٸ� �׳� ��ȯ(4������ ������ ���
//    return { startX * (float)TILE_SIZE_W, startY * (float)TILE_SIZE_H };
//}
//




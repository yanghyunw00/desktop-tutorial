#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <raylib.h>

struct Node
{
    int x, y;
    float gCost; // Distance from start
    float hCost; // Heuristic distance to end
    float fCost; // gCost + hCost
    Node* parent;

    Node(int x = 0, int y = 0) : x(x), y(y), gCost(0), hCost(0), fCost(0), parent(nullptr) {}

    void CalculateFCost()
    {
        fCost = gCost + hCost;
    }
};

struct NodeComparator
{
    bool operator()(Node* a, Node* b)
    {
        if (a->fCost == b->fCost)
        {
            return a->hCost > b->hCost;
        }
        return a->fCost > b->fCost;
    }
};

class PathFinding
{
private:
    int gridSizeX, gridSizeY;
    std::vector<std::vector<float>> heightMap;
    std::vector<std::vector<bool>> walkableMap;
    float maxClimbHeight; // Maximum height difference that can be climbed

    float GetDistance(Node* nodeA, Node* nodeB);
    float GetManhattanDistance(Node* nodeA, Node* nodeB);
    bool IsWalkable(int x, int y);
    bool CanClimb(int fromX, int fromY, int toX, int toY);
    std::vector<Node*> GetNeighbors(Node* node);
    std::vector<Vector2> RetracePath(Node* startNode, Node* endNode);

public:
    PathFinding(int sizeX, int sizeY, float maxClimb = 50.0f);
    ~PathFinding();

    void SetHeightMap(const std::vector<double>& heights);
    void SetWalkable(int x, int y, bool walkable);
    bool IsPositionWalkable(int x, int y);

    std::vector<Vector2> FindPath(Vector2 start, Vector2 end);
    std::vector<Vector2> FindPath(int startX, int startY, int endX, int endY);

    // Visualization helpers
    void DrawPath(const std::vector<Vector2>& path, int tileWidth, int tileHeight, Color color = RED);
    void DrawPathIsometric(const std::vector<Vector2>& path, int tileWidth, int tileHeight, Color color = RED);
};
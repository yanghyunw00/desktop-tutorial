#include "PathFinding.h"
#include <algorithm>
#include <iostream>

PathFinding::PathFinding(int sizeX, int sizeY, float maxClimb)
    : gridSizeX(sizeX), gridSizeY(sizeY), maxClimbHeight(maxClimb)
{
    heightMap.resize(gridSizeY, std::vector<float>(gridSizeX, 0.0f));
    walkableMap.resize(gridSizeY, std::vector<bool>(gridSizeX, true));
}

PathFinding::~PathFinding()
{
    
}

void PathFinding::SetHeightMap(const std::vector<double>& heights)
{
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            if (y * gridSizeX + x < heights.size())
            {
                heightMap[y][x] = static_cast<float>(heights[y * gridSizeX + x]);
            }
        }
    }
}

void PathFinding::SetWalkable(int x, int y, bool walkable)
{
    if (x >= 0 && x < gridSizeX && y >= 0 && y < gridSizeY)
    {
        walkableMap[y][x] = walkable;
    }
}

bool PathFinding::IsPositionWalkable(int x, int y)
{
    if (x < 0 || x >= gridSizeX || y < 0 || y >= gridSizeY)
    {
        return false;
    }
    return walkableMap[y][x];
}

bool PathFinding::IsWalkable(int x, int y)
{
    return IsPositionWalkable(x, y);
}

bool PathFinding::CanClimb(int fromX, int fromY, int toX, int toY)
{
    if (!IsWalkable(fromX, fromY) || !IsWalkable(toX, toY))
    {
        return false;
    }

    float heightDiff = std::abs(heightMap[toY][toX] - heightMap[fromY][fromX]);
    return heightDiff <= maxClimbHeight;
}

float PathFinding::GetDistance(Node* nodeA, Node* nodeB)
{
    int dstX = std::abs(nodeA->x - nodeB->x);
    int dstY = std::abs(nodeA->y - nodeB->y);

    if (dstX > dstY)
    {
        return 14 * dstY + 10 * (dstX - dstY);
    }
    return 14 * dstX + 10 * (dstY - dstX);
}

float PathFinding::GetManhattanDistance(Node* nodeA, Node* nodeB)
{
    return std::abs(nodeA->x - nodeB->x) + std::abs(nodeA->y - nodeB->y);
}

std::vector<Node*> PathFinding::GetNeighbors(Node* node)
{
    std::vector<Node*> neighbors;

    int dx[] = { 0, 0, 1, -1 }; // x 변화량 (좌, 우)
    int dy[] = { 1, -1, 0, 0 }; // y 변화량 (상, 하)

    for (int i = 0; i < 4; i++)
    {
        int checkX = node->x + dx[i];
        int checkY = node->y + dy[i];

        if (checkX >= 0 && checkX < gridSizeX &&
            checkY >= 0 && checkY < gridSizeY &&
            CanClimb(node->x, node->y, checkX, checkY))
        {
            neighbors.push_back(new Node(checkX, checkY));
        }
    }

    return neighbors;
}

std::vector<Vector2> PathFinding::RetracePath(Node* startNode, Node* endNode)
{
    std::vector<Vector2> path;
    Node* currentNode = endNode;

    while (currentNode != startNode)
    {
        path.push_back({ static_cast<float>(currentNode->x), static_cast<float>(currentNode->y) });
        currentNode = currentNode->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Vector2> PathFinding::FindPath(Vector2 start, Vector2 end)
{
    return FindPath(static_cast<int>(start.x), static_cast<int>(start.y),
        static_cast<int>(end.x), static_cast<int>(end.y));
}

std::vector<Vector2> PathFinding::FindPath(int startX, int startY, int endX, int endY)
{
    std::vector<Vector2> path;

    // Boundary checks
    if (startX < 0 || startX >= gridSizeX || startY < 0 || startY >= gridSizeY ||
        endX < 0 || endX >= gridSizeX || endY < 0 || endY >= gridSizeY)
    {
        return path; 
    }

    if (!IsWalkable(startX, startY) || !IsWalkable(endX, endY))
    {
        return path; 
    }

    Node* startNode = new Node(startX, startY);
    Node* targetNode = new Node(endX, endY);

    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openSet;
    std::vector<std::vector<bool>> closedSet(gridSizeY, std::vector<bool>(gridSizeX, false));
    std::vector<std::vector<Node*>> allNodes(gridSizeY, std::vector<Node*>(gridSizeX, nullptr));

    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            allNodes[y][x] = new Node(x, y);
        }
    }

    startNode = allNodes[startY][startX];
    targetNode = allNodes[endY][endX];

    openSet.push(startNode);

    while (!openSet.empty())
    {
        Node* currentNode = openSet.top();
        openSet.pop();

        closedSet[currentNode->y][currentNode->x] = true;

        if (currentNode->x == targetNode->x && currentNode->y == targetNode->y)
        {
            path = RetracePath(startNode, targetNode);
            break;
        }

        std::vector<Node*> neighbors = GetNeighbors(currentNode);

        for (Node* neighbor : neighbors)
        {
            if (closedSet[neighbor->y][neighbor->x])
            {
                delete neighbor;
                continue;
            }

            Node* gridNeighbor = allNodes[neighbor->y][neighbor->x];
            delete neighbor; 

            float newMovementCostToNeighbor = currentNode->gCost + GetDistance(currentNode, gridNeighbor);

            // Add height-based cost
            float heightDiff = std::abs(heightMap[gridNeighbor->y][gridNeighbor->x] -
                heightMap[currentNode->y][currentNode->x]);
            newMovementCostToNeighbor += heightDiff * 0.1f; // Height penalty

            if (newMovementCostToNeighbor < gridNeighbor->gCost || gridNeighbor->gCost == 0)
            {
                gridNeighbor->gCost = newMovementCostToNeighbor;
                gridNeighbor->hCost = GetDistance(gridNeighbor, targetNode);
                gridNeighbor->parent = currentNode;
                gridNeighbor->CalculateFCost();

                if (!closedSet[gridNeighbor->y][gridNeighbor->x])
                {
                    openSet.push(gridNeighbor);
                }
            }
        }
    }

    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            delete allNodes[y][x];
        }
    }

    return path;
}

void PathFinding::DrawPath(const std::vector<Vector2>& path, int tileWidth, int tileHeight, Color color)
{
    for (size_t i = 0; i < path.size(); i++)
    {
        float x = path[i].x * tileWidth;
        float y = path[i].y * tileHeight;

        DrawRectangle(static_cast<int>(x), static_cast<int>(y), tileWidth, tileHeight, color);

        if (i < path.size() - 1)
        {
            Vector2 start = { x + tileWidth / 2.0f, y + tileHeight / 2.0f };
            Vector2 end = { path[i + 1].x * tileWidth + tileWidth / 2.0f,
                          path[i + 1].y * tileHeight + tileHeight / 2.0f };
            DrawLineEx(start, end, 3.0f, color);
        }
    }
}

void PathFinding::DrawPathIsometric(const std::vector<Vector2>& path, int tileWidth, int tileHeight, Color color)
{
    const int TILE_WIDTH_HALF = tileWidth / 2;
    const int TILE_HEIGHT_HALF = tileHeight / 2;

    for (size_t i = 0; i < path.size(); i++)
    {
        int x = static_cast<int>(path[i].x);
        int y = static_cast<int>(path[i].y);

        int isoX = (x - y) * TILE_WIDTH_HALF;
        int isoY = (x + y) * TILE_HEIGHT_HALF;

        DrawCircle(isoX, isoY, 5, color);

        if (i < path.size() - 1)
        {
            int nextX = static_cast<int>(path[i + 1].x);
            int nextY = static_cast<int>(path[i + 1].y);
            int nextIsoX = (nextX - nextY) * TILE_WIDTH_HALF;
            int nextIsoY = (nextX + nextY) * TILE_HEIGHT_HALF;

            DrawLineEx({ static_cast<float>(isoX), static_cast<float>(isoY) },
                { static_cast<float>(nextIsoX), static_cast<float>(nextIsoY) },
                2.0f, color);
        }
    }
}
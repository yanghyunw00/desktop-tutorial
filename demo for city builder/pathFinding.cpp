#include "PathFinding.h"
#include <algorithm>
#include <iostream>

PathFinding::PathFinding(int sizeX, int sizeY, float maxClimb)
    : gridSizeX(sizeX), gridSizeY(sizeY), maxClimbHeight(maxClimb)
{
    heightMap.resize(gridSizeY, std::vector<float>(gridSizeX, 0.0f));
    walkableMap.resize(gridSizeY, std::vector<bool>(gridSizeX, true));

    allNodes.resize(gridSizeY, std::vector<Node*>(gridSizeX, nullptr));
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            allNodes[y][x] = new Node(x, y);
        }
    }
}

PathFinding::~PathFinding()
{
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            delete allNodes[y][x];
        }
    }
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

void PathFinding::SetCostMap(const std::vector<float>& costs)
{
    costMap.resize(gridSizeY, std::vector<float>(gridSizeX, 999.9f));
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            costMap[y][x] = costs[y * gridSizeX + x];
        }
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

float PathFinding::GetManhattanDistance(Node* nodeA, Node* nodeB)
{
    return std::abs(nodeA->x - nodeB->x) + std::abs(nodeA->y - nodeB->y);
}

std::vector<Node*> PathFinding::GetNeighbors(Node* node)
{
    std::vector<Node*> neighbors;

    int dx[] = { 0, 0, 1, -1 }; // x  좌, 우
    int dy[] = { 1, -1, 0, 0 }; // y  상, 하

    for (int i = 0; i < 4; i++)
    {
        int checkX = node->x + dx[i];
        int checkY = node->y + dy[i];

        if (checkX >= 0 && checkX < gridSizeX &&
            checkY >= 0 && checkY < gridSizeY &&
            CanClimb(node->x, node->y, checkX, checkY))
        {
            neighbors.push_back(allNodes[checkY][checkX]);
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
    path.push_back({ static_cast<float>(startNode->x), static_cast<float>(startNode->y) });

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

    //초기화
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            Node* node = allNodes[y][x];
            node->gCost = 0; 
            node->hCost = 0;
            node->parent = nullptr;
        }
    }

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
                continue;
            }

            float newMovementCostToNeighbor = currentNode->gCost + GetManhattanDistance(currentNode, neighbor);

            float heightDiff = std::abs(heightMap[neighbor->y][neighbor->x] -
                heightMap[currentNode->y][currentNode->x]);
            newMovementCostToNeighbor += heightDiff * 0.1f; 

            //tile에 따른 cost계산시 -- 미완성
            //float moveCost = costMap[neighbor->y][neighbor->x]; 
            //float newMovementCostToNeighbor = currentNode->gCost + moveCost; 
            //float heightDiff = std::abs(heightMap[neighbor->y][neighbor->x] - heightMap[currentNode->y][currentNode->x]);
            //newMovementCostToNeighbor += heightDiff * 0.1f;

            if (newMovementCostToNeighbor < neighbor->gCost || neighbor->gCost == 0)
            {
                neighbor->gCost = newMovementCostToNeighbor;
                neighbor->hCost = GetManhattanDistance(neighbor, targetNode);
                neighbor->parent = currentNode;
                neighbor->CalculateFCost();

                if (!closedSet[neighbor->y][neighbor->x])
                {
                    openSet.push(neighbor);
                }
            }
        }
    }



    return path;
}

void PathFinding::DrawPathIsometric(const std::vector<Vector2>& path, int tileWidth, int tileHeight,
    int isoOriginX, int isoOriginY, Color color)
{
    const int TILE_WIDTH_HALF = tileWidth / 2;
    const int TILE_HEIGHT_HALF = tileHeight / 2;

    const int TILE_VISUAL_OFFSET_Y = 0;

    for (size_t i = 0; i < path.size(); i++)
    {
        int x = static_cast<int>(path[i].x);
        int y = static_cast<int>(path[i].y);
        float currentHeight = heightMap[y][x];

        int isoX = isoOriginX + (x - y) * TILE_WIDTH_HALF;
        int isoY = isoOriginY + (x + y) * TILE_HEIGHT_HALF - static_cast<int>(currentHeight);

        int circleCenterX = isoX + TILE_WIDTH_HALF;
        int circleCenterY = isoY + TILE_HEIGHT_HALF + TILE_VISUAL_OFFSET_Y;

        DrawCircle(circleCenterX, circleCenterY, 5, color);

        if (i < path.size() - 1)
        {
            int nextX = static_cast<int>(path[i + 1].x);
            int nextY = static_cast<int>(path[i + 1].y);
            float nextHeight = heightMap[nextY][nextX];

            int nextIsoX = isoOriginX + (nextX - nextY) * TILE_WIDTH_HALF;
            int nextIsoY = isoOriginY + (nextX + nextY) * TILE_HEIGHT_HALF - static_cast<int>(nextHeight);

            int nextCircleCenterX = nextIsoX + TILE_WIDTH_HALF;
            int nextCircleCenterY = nextIsoY + TILE_HEIGHT_HALF + TILE_VISUAL_OFFSET_Y;

            DrawLineEx({ (float)circleCenterX, (float)circleCenterY },
                { (float)nextCircleCenterX, (float)nextCircleCenterY },
                2.0f, color);
        }
    }
}
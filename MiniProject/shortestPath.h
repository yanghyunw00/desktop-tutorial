#pragma once
#include "pathFinding.h"
#include "raylib.h"
#include <vector>


class ShortPath {
private:
    std::vector<Vector2> path;
public:

    void SetPath(const std::vector<Vector2>& newPath);
    void Draw() const;
    void Clear();
};

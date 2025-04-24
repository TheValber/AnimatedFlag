#pragma once
#include <unordered_map>
#include <vector>
#include "PMat.hpp"

struct GridCoord {
    int x, y, z;

    bool operator==(const GridCoord& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

namespace std {
    template <>
    struct hash<GridCoord> {
        size_t operator()(const GridCoord& g) const {
            return ((std::hash<int>()(g.x) ^ (std::hash<int>()(g.y) << 1)) >> 1) ^ (std::hash<int>()(g.z) << 1);
        }
    };
}

class SpatialHashGrid {
public:
    SpatialHashGrid(float cellSize);

    void insert(PMat* pmat);
    std::vector<PMat*> queryNeighbors(PMat* pmat, float radius);
    void clear();

private:
    float cellSize;
    std::unordered_map<GridCoord, std::vector<PMat*>> grid;

    GridCoord getGridCoord(const Point& pos) const;
};

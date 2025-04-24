#include "../include/SpatialHashGrid.hpp"
#include <cmath>

SpatialHashGrid::SpatialHashGrid(float cellSize) : cellSize(cellSize) {}

void SpatialHashGrid::clear() {
    grid.clear();
}

GridCoord SpatialHashGrid::getGridCoord(const Point& pos) const {
    return {
        static_cast<int>(std::floor(pos.getX() / cellSize)),
        static_cast<int>(std::floor(pos.getY() / cellSize)),
        static_cast<int>(std::floor(pos.getZ() / cellSize))
    };
}

void SpatialHashGrid::insert(PMat* pmat) {
    GridCoord coord = getGridCoord(pmat->getPos());
    grid[coord].push_back(pmat);
}

std::vector<PMat*> SpatialHashGrid::queryNeighbors(PMat* pmat, float radius) {
    std::vector<PMat*> neighbors;
    GridCoord base = getGridCoord(pmat->getPos());
    int range = static_cast<int>(std::ceil(radius / cellSize));

    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            for (int dz = -range; dz <= range; ++dz) {
                GridCoord neighbor = {base.x + dx, base.y + dy, base.z + dz};
                if (grid.find(neighbor) != grid.end()) {
                    for (PMat* candidate : grid[neighbor]) {
                        if (candidate != pmat) {
                            Vect delta = candidate->getPos() - pmat->getPos();
                            if (delta.norm() < radius) {
                                neighbors.push_back(candidate);
                            }
                        }
                    }
                }
            }
        }
    }
    return neighbors;
}

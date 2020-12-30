#include "World.h"

#include "../Constants.h"

#include <gf/Noises.h>

namespace sail
{

    World::World()
    : m_terrain({MapSize, MapSize})
    , m_winds({MapSize, MapSize})
    , m_random()
    {
        generate();
    }

    static double valueWithWaterLevel(double value, double waterLevel) {
        if (value < waterLevel)
            return value / waterLevel * 0.5;

        return (value - waterLevel) / (1.0 - waterLevel) * 0.5 + 0.5;
    }

    float World::averageWind(const gf::Array2D<float> data, unsigned int col, unsigned int row)
    {
        float weightedSum = 0.0f;
        float totalWeight = 0.0f;
        int minCol = (col > 0) ? -1 : 0;
        int maxCol = (col < data.getCols() - 1) ? 1 : 0;
        int minRow = (row > 0) ? -1 : 0;
        int maxRow = (row < data.getRows() - 1) ? 1 : 0;
        for (int i = minCol; i <= maxCol; i++)
        {
            for (int j = minRow; j <= maxRow; j++)
            {
                if (i == 0 && j == 0)
                    continue;
                float val = data({ col + i, row + j});
                totalWeight += val;
                weightedSum += Angles[i + 1][j + 1] * val;
            }
        }
        return weightedSum / totalWeight;
    }

    int clusterizeRec(const gf::Array2D<float> data, int size, int depth, unsigned int col, unsigned int row,
            gf::Array2D<float> c0, gf::Array2D<float> c1, gf::Array2D<float> c2)
    {
        int cSize = size / (2 ^ depth);
        if (size == 256)
        {
            float sum = 0.0f;
            for (int i = col; i < col + size; i++)
            {
                for (int j = row; j < row + size; j++)
                {
                    sum += data({ col, row });
                }
            }
            return sum / (size * size);
        }

        float tl = clusterizeRec(data, size, depth + 1, col, row, c0, c1, c2);
        float tr = clusterizeRec(data, size, depth + 1, col + cSize, row, c0, c1, c2);
        float bl = clusterizeRec(data, size, depth + 1, col, row + cSize, c0, c1, c2);
        float br = clusterizeRec(data, size, depth + 1, col + cSize, row + cSize, c0, c1, c2);

        // TODO : unfinished, do not use this -> need to make the clustering fully dynamic
    }

    void generateWinds(const gf::Array2D<float>& terrain)
    {
        gf::Array2D<float> RPMap ({ MapSize, MapSize });

        for (auto row : terrain.getRowRange())
        {
            for (auto col : terrain.getColRange())
            {
                float height = terrain({ col, row });
                if (height < 0.5f)
                    RPMap({ col, row }) = 0.0f;
                else
                    RPMap({ col, row }) = height * 20.0f + 30.0f; // 40 -> 50
            }
        }

        gf::Array2D<float> clustersMap_0 ({ 2, 2 });
        gf::Array2D<float> clustersMap_1 ({ 4, 4 });
        gf::Array2D<float> clustersMap_2 ({ 8, 8 });
    }

    void World::generate()
    {
        gf::SimplexNoise2D simplex(m_random);
        gf::FractalNoise2D fractal(simplex, 1);

        for (auto row : m_terrain.getRowRange())
        {
            double y = static_cast<double>(row) / m_terrain.getRows() * Scale;
            for (auto col : m_terrain.getColRange())
            {
                double x = static_cast<double>(col) / m_terrain.getCols() * Scale;
                m_terrain({ col, row }) = fractal.getValue(x, y);
            }
        }

        float min = *std::min_element(m_terrain.begin(), m_terrain.end());
        float max = *std::max_element(m_terrain.begin(), m_terrain.end());

        for (auto& elevation : m_terrain)
        {
            elevation = (elevation - min) / (max - min);
            elevation = valueWithWaterLevel(elevation, SeaLevel);
            assert(0.0 <= elevation && elevation <= 1.0);
        }

        gf::Vector2d startingP;
        unsigned col, row;
        do {
           startingP.x = m_random.computeUniformFloat(MapMinBorder, MapMaxBorder);
           startingP.y = m_random.computeUniformFloat(MapMinBorder, MapMaxBorder);

            col = static_cast<unsigned>(startingP.x / TileDegree);
            row = static_cast<unsigned>(startingP.y / TileDegree);
        }
        while(m_terrain({ row, col }) > 0.25f);

        m_startingPosition = startingP;
    }

    bool World::isOnLand(double x, double y)
    {
        auto col = static_cast<unsigned>(x / TileDegree);
        auto row = static_cast<unsigned>(y / TileDegree);

        return m_terrain({ row, col }) > 0.5f;
    }

    gf::Array2D<float>& World::getTerrain()
    {
        return m_terrain;
    }

    gf::Vector2f World::getStartingPosition()
    {
        return m_startingPosition;
    }

    /*
     * Plan :
     * - generating the whole world and sending the whole terrain to the player before the game
     * - cutting the wind-map in chunks and sending them to player when they get close enough to activate them
     * - keeping track of the chunks we already sent to players
     */


}
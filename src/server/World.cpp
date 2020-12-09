#include "World.h"

#include <gf/Noises.h>

namespace sail
{

    World::World()
    : m_terrain({Size, Size})
    , m_wind({Size, Size})
    , m_random()
    {
        generate();
    }

    static double valueWithWaterLevel(double value, double waterLevel) {
        if (value < waterLevel) {
            return value / waterLevel * 0.5;
        }

        return (value - waterLevel) / (1.0 - waterLevel) * 0.5 + 0.5;
    }

    void World::generate()
    {
        gf::SimplexNoise2D simplex(m_random);
        gf::FractalNoise2D fractal(simplex, 1);

        for (auto row : m_terrain.getRowRange()) {
            double y = static_cast<double>(row) / m_terrain.getRows() * Scale;
            for (auto col : m_terrain.getColRange()) {
                double x = static_cast<double>(col) / m_terrain.getCols() * Scale;
                m_terrain({ col, row }) = fractal.getValue(x, y);
            }
        }

        float min = *std::min_element(m_terrain.begin(), m_terrain.end());
        float max = *std::max_element(m_terrain.begin(), m_terrain.end());

        for (auto& elevation : m_terrain) {
            elevation = (elevation - min) / (max - min);
            elevation = valueWithWaterLevel(elevation, SeaLevel);
            assert(0.0 <= elevation && elevation <= 1.0);
        }
    }

    gf::Array2D<float> World::getTerrain()
    {
        return m_terrain;
    }

    /*
     * Plan :
     * - generating the whole world and sending the whole terrain to the player before the game
     * - cutting the wind-map in chunks and sending them to player when they get close enough to activate them
     * - keeping track of the chunks we already sent to players
     */


}
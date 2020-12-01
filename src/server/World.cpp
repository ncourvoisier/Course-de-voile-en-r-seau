#include "World.h"

#include <gf/Noises.h>

namespace sail
{

    World::World()
    : m_terrain({Size, Size})
    , m_wind({Size, Size})
    , m_random()
    {

    }

    void World::generate()
    {
        gf::SimplexNoise2D simplex(m_random);
        gf::FractalNoise2D fractal(simplex, 1);

        for (auto row : m_wind.getRowRange()) {
            double y = static_cast<double>(row) / m_wind.getRows() * Scale;
            for (auto col : m_wind.getColRange()) {
                double x = static_cast<double>(col) / m_wind.getCols() * Scale;
                m_wind({ col, row }) = fractal.getValue(x, y) * M_PI * 2;
            }
        }
    }

    /*
     * Plan :
     * - generating the whole world and sending the whole terrain to the player before the game
     * - cutting the wind-map in chunks and sending them to player when they get close enough to activate them
     * - keeping track of the chunks we already sent to players
     */


}
#include "World.h"

#include "../Constants.h"

#include <gf/Noises.h>
#include <iostream>

namespace sail
{

    World::World()
    : m_terrain({MapSize, MapSize})
    , m_windDirection({MapSize, MapSize})
    , m_windSpeed({MapSize, MapSize})
    , m_random()
    {
        generate();
    }

    static double valueWithWaterLevel(double value, double waterLevel) {
        if (value < waterLevel)
            return value / waterLevel * 0.5;

        return (value - waterLevel) / (1.0 - waterLevel) * 0.5 + 0.5;
    }

    void World::generate()
    {
        gf::SimplexNoise2D simplex(m_random);
        gf::FractalNoise2D fractal(simplex, 1);

        /// Terrain ///

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

        /// Wind Direction ///

        m_random.getEngine().discard(1);
        gf::SimplexNoise2D windDNoise(m_random);

        for (auto row : m_windDirection.getRowRange())
        {
            double y = static_cast<double>(row) / m_windDirection.getRows() * WindScale;
            for (auto col : m_windDirection.getColRange())
            {
                double x = static_cast<double>(col) / m_windDirection.getCols() * WindScale;
                m_windDirection({ col, row }) = ((windDNoise.getValue(x, y) * 0.5f) + 0.5f) * M_PI * 2.0f;
            }
        }

        /// Wind Speed ///

        m_random.getEngine().discard(1);
        gf::SimplexNoise2D windSNoise(m_random);

        for (auto row : m_windSpeed.getRowRange())
        {
            double y = static_cast<double>(row) / m_windSpeed.getRows() * Scale;
            for (auto col : m_windSpeed.getColRange())
            {
                double x = static_cast<double>(col) / m_windSpeed.getCols() * Scale;
                m_windSpeed({ col, row }) = ((windSNoise.getValue(x, y) * 0.5f) + 0.5f) * 29.0f + 1.0f;
            }
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

    gf::Array2D<float>& World::getWindDirection()
    {
        return m_windDirection;
    }

    gf::Array2D<float>& World::getWindSpeed()
    {
        return m_windSpeed;
    }

    /*
     * Plan :
     * - generating the whole world and sending the whole terrain to the player before the game
     * - cutting the wind-map in chunks and sending them to player when they get close enough to activate them
     * - keeping track of the chunks we already sent to players
     */


}
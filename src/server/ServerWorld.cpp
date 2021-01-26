#include "ServerWorld.h"

#include "../Constants.h"

#include <gf/Noises.h>

namespace sail
{

    ServerWorld::ServerWorld()
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

    void ServerWorld::generate()
    {
        gf::SimplexNoise2D simplex(m_random);
        gf::FractalNoise2D fractal(simplex, 1);

        /// ClientWorld ///

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
            double y = static_cast<double>(row) / m_windSpeed.getRows() * WindScale;
            for (auto col : m_windSpeed.getColRange())
            {
                double x = static_cast<double>(col) / m_windSpeed.getCols() * WindScale;
                m_windSpeed({ col, row }) = ((windSNoise.getValue(x, y) * 0.5f) + 0.5f) * 40.0f + 10.0f; // Wind Speed : 10 m/s -> 50 m/s
            }
        }

        m_startingPosition = randomWaterLocation();
        m_endingPosition = randomWaterLocation();
    }

    gf::Vector2d ServerWorld::randomWaterLocation()
    {
        gf::Vector2d loc;
        unsigned col, row;
        do {
            loc.x = m_random.computeUniformFloat(MapMinBorder, MapMaxBorder);
            loc.y = m_random.computeUniformFloat(MapMinBorder, MapMaxBorder);

            col = static_cast<unsigned>(loc.x / TileDegree);
            row = static_cast<unsigned>(loc.y / TileDegree);
        }
        while(m_terrain({ row, col }) > 0.25f);
        return loc;
    }

    gf::Vector2f ServerWorld::getStartingPosition()
    {
        return m_startingPosition;
    }

    gf::Vector2f ServerWorld::getEndingPosition()
    {
        return m_endingPosition;
    }

    const gf::Array2D<float>& ServerWorld::getWindDirection() const
    {
        return m_windDirection;
    }

    const gf::Array2D<float> & ServerWorld::getWindSpeed() const
    {
        return m_windSpeed;
    }

    const gf::Array2D<float>& ServerWorld::getTerrain() const
    {
        return m_terrain;
    }

    const gf::Vector2d ServerWorld::getStartingPosition() const
    {
        return m_startingPosition;
    }

}
#ifndef SAILINSANE_WORLD_H
#define SAILINSANE_WORLD_H

#include <gf/Array2D.h>
#include <gf/Random.h>
#include "../Protocol.h"
#include "../physics/Wind.h"

namespace sail
{

    class World
    {
    public:
        static constexpr double Scale = 15.0;
        static constexpr double WindScale = 5.0;
        static constexpr double SeaLevel = 0.65;

        World();

        gf::Array2D<float>& getTerrain();
        gf::Vector2f getStartingPosition();
        gf::Vector2f getEndingPosition();

        gf::Vector2d randomWaterLocation();
        bool isOnLand(double x, double y);
        Wind getWindAtPosition(double x, double y);

        gf::Array2D<float>& getWindDirectionArray();
        gf::Array2D<float>& getWindSpeedArray();

    private:
        void generate();

    private:
        gf::Random m_random;

        gf::Array2D<float> m_windDirection;
        gf::Array2D<float> m_windSpeed;
        gf::Array2D<float> m_terrain;

        gf::Vector2f m_startingPosition;
        gf::Vector2f m_endingPosition;
    };

}

#endif //SAILINSANE_WORLD_H

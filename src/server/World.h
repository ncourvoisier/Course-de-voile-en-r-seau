#ifndef SAILINSANE_WORLD_H
#define SAILINSANE_WORLD_H

#include <gf/Array2D.h>
#include <gf/Random.h>
#include "../Protocol.h"

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

        bool isOnLand(double x, double y);

        gf::Array2D<float>& getWindDirection();
        gf::Array2D<float>& getWindSpeed();

    private:
        void generate();

    private:
        gf::Random m_random;

        gf::Array2D<float> m_windDirection;
        gf::Array2D<float> m_windSpeed;
        gf::Array2D<float> m_terrain;

        gf::Vector2d m_startingPosition;
    };

}

#endif //SAILINSANE_WORLD_H

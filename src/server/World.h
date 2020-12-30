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
        static constexpr double SeaLevel = 0.65;

        World();

        gf::Array2D<float>& getTerrain();
        gf::Vector2f getStartingPosition();

        bool isOnLand(double x, double y);

    private:
        void generate();

        float averageWind(const gf::Array2D<float> data, unsigned int col, unsigned int row);

    private:
        gf::Random m_random;

        gf::Array2D<WindData> m_winds;
        gf::Array2D<float> m_terrain;

        gf::Vector2d m_startingPosition;

        static constexpr float Angles[3][3] = {
                { 7 * M_PI_4, 0, M_PI_4 },
                { 3 * M_PI_2, 0, M_PI_2 },
                { 5 * M_PI_4, M_PI, 3 * M_PI_4 }
        };
    };

}

#endif //SAILINSANE_WORLD_H

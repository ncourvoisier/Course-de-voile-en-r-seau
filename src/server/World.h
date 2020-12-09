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
        static constexpr unsigned Size = 1500;
        static constexpr float TileSize = 8.0f;
        static constexpr unsigned Edge = 80;
        static constexpr float WorldMin = Edge * TileSize;
        static constexpr float WorldMax = (Size - Edge) * TileSize;
        static constexpr double Scale = 10.0;
        static constexpr double SeaLevel = 0.65;

        World();

        gf::Array2D<float> getTerrain();

    private:
        void generate();

    private:
        gf::Array2D<float> m_terrain;
        gf::Array2D<float> m_wind;
        gf::Random m_random;
    };

}

#endif //SAILINSANE_WORLD_H

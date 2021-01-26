#ifndef SAILINSANE_SERVERWORLD_H
#define SAILINSANE_SERVERWORLD_H

#include <gf/Array2D.h>
#include <gf/Random.h>
#include "../Protocol.h"
#include "../physics/Wind.h"
#include "../physics/World.h"

namespace sail
{

    class ServerWorld: public World
    {
    public:
        static constexpr double Scale = 15.0;
        static constexpr double WindScale = 5.0;
        static constexpr double SeaLevel = 0.65;

        ServerWorld();

        gf::Vector2f getStartingPosition();
        gf::Vector2f getEndingPosition();
        gf::Vector2d randomWaterLocation();

        const gf::Array2D<float>& getTerrain() const;
        const gf::Array2D<float>& getWindDirection() const;
        const gf::Array2D<float>& getWindSpeed() const;
        const gf::Vector2d getStartingPosition() const override;

    private:
        void generate();

    private:
        gf::Random m_random;

        gf::Array2D<float> m_windDirection;
        gf::Array2D<float> m_windSpeed;
        gf::Array2D<float> m_terrain;

        gf::Vector2d m_startingPosition;
        gf::Vector2d m_endingPosition;
    };

}

#endif //SAILINSANE_SERVERWORLD_H

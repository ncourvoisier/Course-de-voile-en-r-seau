#ifndef SAILINSANE_WORLD_H
#define SAILINSANE_WORLD_H

#include <gf/Array2D.h>

namespace sail
{

    class World
    {
    public:
        virtual const gf::Array2D<float>& getWindDirection() const = 0;
        virtual const gf::Array2D<float>& getWindSpeed() const = 0;
        virtual const gf::Array2D<float>& getTerrain() const = 0;
        virtual const gf::Vector2d getStartingPosition() const = 0;
    };

}

#endif //SAILINSANE_WORLD_H

#ifndef SAILINSANE_CONVERSIONS_H
#define SAILINSANE_CONVERSIONS_H

#include <gf/Vector.h>

namespace sail
{

    gf::Vector2f toFloatVectorPosition(double x, double y)
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }

}

#endif //SAILINSANE_CONVERSIONS_H

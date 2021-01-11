#ifndef SAILINSANE_CONSTANTS_H
#define SAILINSANE_CONSTANTS_H

#include <gf/Time.h>

namespace sail
{

    const unsigned WorldScale = 100000;
    constexpr unsigned MapSize = 2000;
    constexpr unsigned MapEdge = 100;
    constexpr double TileDegree = 0.00005;
    constexpr float MapMinBorder = MapEdge * TileDegree;
    constexpr float MapMaxBorder = (MapSize - MapEdge) * TileDegree;

    static constexpr int TicksPerSecond = 30;
    static constexpr int TickLength = 1000 / TicksPerSecond;
    static constexpr gf::Time FrameTime = gf::milliseconds(TickLength);

}

#endif //SAILINSANE_CONSTANTS_H

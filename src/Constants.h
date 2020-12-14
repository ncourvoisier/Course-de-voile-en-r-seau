#ifndef SAILINSANE_CONSTANTS_H
#define SAILINSANE_CONSTANTS_H

namespace sail
{

    const unsigned WorldScale = 100000;
    constexpr unsigned MapSize = 2000;
    constexpr unsigned MapEdge = 100;
    constexpr double TileDegree = 0.00005;
    constexpr float MapMinBorder = MapEdge * TileDegree;
    constexpr float MapMaxBorder = (MapSize - MapEdge) * TileDegree;

}

#endif //SAILINSANE_CONSTANTS_H

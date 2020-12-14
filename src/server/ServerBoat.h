#ifndef SAILINSANE_SERVERBOAT_H
#define SAILINSANE_SERVERBOAT_H

#include <gf/Shapes.h>
#include <gf/Vector.h>
#include "../Protocol.h"
#include "../physics/Boat.h"
#include "../physics/Wind.h"
#include "../physics/Physics.h"

namespace sail
{

    class ServerBoat : public Boat
    {
    public:
        ServerBoat() = default; // Invalid boat
        ServerBoat(gf::Id playerId, double x, double y);

        BoatData getBoatData();

    private:
        gf::Id m_playerId;
    };

}


#endif //SAILINSANE_SERVERBOAT_H

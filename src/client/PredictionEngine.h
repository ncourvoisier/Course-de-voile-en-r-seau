#ifndef SAILINSANE_PREDICTIONENGINE_H
#define SAILINSANE_PREDICTIONENGINE_H

#include <deque>
#include "../Protocol.h"
#include "../physics/Boat.h"
#include "../physics/BoatController.h"
#include "ClientWorld.h"

namespace sail
{

    class PredictionEngine
    {
    public:
        PredictionEngine(ClientBoat& boat, ClientWorld& terrain);

        void pushAction(PlayerAction& action);
        void reconciliate(unsigned int lastAckActionId);

        void enable();
        void disable();

    private:
        void update(gf::Time dt);

    private:
        bool m_enabled;

        unsigned int m_lastActionId;
        std::deque<PlayerAction> m_actionsHistory;

        ClientBoat& m_boat;
        ClientWorld& m_terrain;

        BoatController m_boatController;
    };

}

#endif //SAILINSANE_PREDICTIONENGINE_H

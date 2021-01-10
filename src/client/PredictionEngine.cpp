#include "PredictionEngine.h"

namespace sail
{

    PredictionEngine::PredictionEngine(ClientBoat& boat, Terrain& terrain)
    : m_boat(boat)
    , m_terrain(terrain)
    , m_lastActionId(0)
    , m_actionsHistory()
    , m_boatController()
    {

    }

    void PredictionEngine::update(gf::Time dt)
    {
        m_boatController.updateBoatPosition(
                m_boat,
                m_terrain.getWindSpeedArray(),
                m_terrain.getWindDirectionArray(),
                dt);
    }

    void PredictionEngine::pushAction(PlayerAction &action)
    {
        action.id = m_lastActionId++;
        m_actionsHistory.push_back(action);
        m_boatController.processPlayerAction(m_boat, action);
    }

    void PredictionEngine::reconciliate(unsigned int lastAckActionId)
    {
        for (auto it = m_actionsHistory.begin(); it != m_actionsHistory.end(); *it++)
        {
            if (it->id <= lastAckActionId)
                continue;
            m_boatController.processPlayerAction(m_boat, *it);
            update(gf::milliseconds(50)); // TODO : to change, testing
        }
        m_actionsHistory.clear();
    }

}
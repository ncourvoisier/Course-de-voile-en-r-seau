#include "PredictionEngine.h"

namespace sail
{

    PredictionEngine::PredictionEngine(/*ClientBoat& boat, Terrain& terrain*/)
    : /*m_boat(boat)
    , m_terrain(terrain)
    , */m_lastActionId(0)
    , m_actionsHistory()
    , m_boatController()
    , m_enabled(true)
    {

    }

    void PredictionEngine::update(gf::Time dt)
    {
        /*m_boatController.updateBoatPosition(
                m_boat,
                m_terrain.getWindSpeedArray(),
                m_terrain.getWindDirectionArray(),
                dt);*/
    }

    void PredictionEngine::pushAction(PlayerAction &action)
    {
        action.id = m_lastActionId++;
        /*if (m_enabled)
        {
            m_actionsHistory.push_back(action);
            m_boatController.processPlayerAction(m_boat, action);
            update(sail::FrameTime);
        }*/
    }

    void PredictionEngine::reconciliate(unsigned int lastAckActionId)
    {
        /*if (! m_enabled)
            return;
        while (! m_actionsHistory.empty() && m_actionsHistory.front().id <= lastAckActionId)
        {
            m_actionsHistory.pop_front();
        }

        for (auto it = m_actionsHistory.begin(); it != m_actionsHistory.end(); *it++)
        {
            m_boatController.processPlayerAction(m_boat, *it);
            update(sail::FrameTime);
        }*/
    }

    void PredictionEngine::enable()
    {
        m_enabled = true;
    }

    void PredictionEngine::disable()
    {
        m_enabled = false;
        m_actionsHistory.clear();
    }

}
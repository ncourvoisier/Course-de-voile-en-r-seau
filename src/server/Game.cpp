#include <iostream>

#include <gf/Random.h>
#include <gf/Clock.h>

#include "Managers.h"

#include "Game.h"
#include "../physics/Physics.h"
#include "GameMessages.h"

namespace sail
{

    Game::Game(int neededPlayers)
    : m_neededPlayers(neededPlayers)
    , m_playersNb(0)
    , m_world()
    , m_boatController(m_world)
    , m_onlinePlayers()
    , m_started(false)
    , m_fixedWind()
    {
    }

    void Game::start()
    {
        m_started = true;
    }

    bool Game::isStarted()
    {
        return m_started;
    }

    bool Game::connectPlayer(Player &player, gf::Id id, std::string userName)
    {
        if (m_started)
            return false;
        player.connect(id, userName, m_world);
        return ++m_playersNb == m_neededPlayers;
    }

    bool Game::disconnectPlayer(Player& player)
    {
        for (auto it = m_onlinePlayers.begin(); it != m_onlinePlayers.end(); *it++)
        {
            if (*it == player)
            {
                m_offlinePlayers.push_back(std::move(*it));
                m_onlinePlayers.erase(it);
                break;
            }
        }
        m_offlinePlayers.back().disconnect();
        if (m_onlinePlayers.empty())
            m_started = false;
        return m_started;
    }

    std::vector<Player>& Game::getOnlinePlayers()
    {
        return m_onlinePlayers;
    }

    std::vector<Player>& Game::getOfflinePlayers()
    {
        return m_offlinePlayers;
    }

    void Game::playerAction(Player& player, PlayerAction& action)
    {
        if (! m_started )
            return;

        ServerBoat& boat = player.getBoat();

        m_boatController.processPlayerAction(boat, action);

        player.setLastAckActionId(action.id);
    }

    WorldData Game::getWorldData()
    {
        return { m_world.getTerrain(),
                 m_world.getWindDirection(),
                 m_world.getWindSpeed(),
                 m_world.getStartingPosition(),
                 m_world.getEndingPosition() };
    }

    GameState Game::update(gf::Time dt)
    {
        std::vector<BoatData> boatsData;

        for (auto& player : m_onlinePlayers)
        {
            if (player.isConnected())
            {
                 if (! m_boatController.updateBoatPosition(
                        player.getBoat(),
                        dt))
                 {
                     PlayerDied pDIed;
                     pDIed.id = player.getId();
                     gMessageManager().sendMessage(&pDIed);
                 }
                playerFinished(player);
                boatsData.push_back(player.getBoat().getBoatData());
            }
        }

        return { boatsData };
    }

    void Game::playerFinished(Player& player)
    {
        Boat& boat = player.getBoat();
        if (! player.finished() && gf::euclideanDistance({static_cast<float>(boat.getLongitude()), static_cast<float>(boat.getLatitude()) },
                m_world.getEndingPosition()) < 0.0005f) // == 50m
        {
            PlayerFinished pFinished;
            pFinished.id = player.getId();
            gMessageManager().sendMessage(&pFinished);
            player.setFinished();
        }
    }

}
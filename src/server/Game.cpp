//
// Created by augustin on 01/11/2020.
//

#include <iostream>
#include <thread>

#include <gf/Random.h>
#include <gf/Clock.h>

#include "Managers.h"

#include "Game.h"
#include "../physics/Physics.h"
#include "GameMessages.h"

namespace sail
{

    Game::Game(int neededPlayers)
    : m_onlinePlayers()
    , m_started(false)
    , m_playersNb(0)
    , m_fixedWind()
    , m_world()
    , m_boatController(m_world)
    , m_neededPlayers(neededPlayers)
    {

    }

    void Game::start()
    {
        m_started = true;
        //runSimulation();
    }

    bool Game::isStarted()
    {
        return m_started;
    }

    /*  gf::Id Game::addPlayer(gf::TcpSocket socket)
      {
          gf::Random rand;
          gf::Id newId {rand.computeId()};
          Player player(std::move(socket), newId);
          m_players.push_back(std::move(player));
          return newId;
      }
  */
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
        //std::cout << "Action of : " << player.getName() << "\n";
        ServerBoat& boat = player.getBoat();

        //m_simulationMutex.lock(); Works fine without mutex

        m_boatController.processPlayerAction(boat, action);

        player.setLastAckActionId(action.id);
        //m_simulationMutex.unlock();
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

    void Game::runSimulation()
    {
        m_simulationRunning = true;
        m_simulationThread = std::thread(&Game::simulation, this); //TODO : might be good to close this someday
    }

    void Game::simulation()
    {
        gf::Clock clock;
        double updateSec = 0.05;
        double physicGranularity = 0.01; // Any granularity above this one can * sometimes * creates very unpredictable movements
        double loopsAmount = updateSec / physicGranularity; // TODO : some experimental things here

        while(m_simulationRunning) {
            clock.restart();
            m_simulationMutex.lock();

            for (auto& player : m_onlinePlayers) {
                if (player.isConnected())
                {
                    Wind wind = m_world.getWindAtPosition(player.getBoat().getLongitude(), player.getBoat().getLatitude());
                    for (int i = 0; i < loopsAmount; i++)
                    {
                        sailing_physics_update(player.getBoat(), wind, physicGranularity);
                    }
                    playerFinished(player);
                }
            }

            m_simulationMutex.unlock();
            double elapsed = clock.restart().asSeconds();
            std::cout << "Simulation " << (elapsed * 100.0) / updateSec << "% loaded\n";
            double sleepTime = updateSec - elapsed;

            assert(sleepTime >= 0); // Assert if the simulation is late compared to real world time (shouldn't be)
            // With granularity = 0.01s, 50ms requires 5 loops, taking around 0.004-0.006ms per player in the game

            std::chrono::milliseconds span((int)(sleepTime * 1000));
            std::this_thread::sleep_for(span);
        }

    }

}
//
// Created by augustin on 01/11/2020.
//

#include <iostream>
#include <thread>

#include <gf/Random.h>
#include <gf/Clock.h>

#include "Game.h"
#include "../physics/Physics.h"

namespace sail
{

    Game::Game()
    : m_players()
    , m_started(false)
    , m_playersNb(0)
    , m_boatControl()
    , m_fixedWind()
    {

    }

    void Game::start()
    {
        m_started = true;
        runSimulation();
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
            return false; // TODO : detail return type
        player.connect(id, userName);
        if (++m_playersNb == NeededPlayers)
        {
            start();
            return true;
        }
        return false;
    }

    std::vector<Player>& Game::getPlayers()
    {
        return m_players;
    }

    void Game::playerAction(Player& player, PlayerAction action)
    {
        if (! m_started )
            return;
        std::cout << "Action of : " << player.getName() << "\n";
        ServerBoat& boat = player.getBoat();

        m_simulationMutex.lock();

        switch (action.sailAction)
        {
            case PlayerAction::Type::Right: // TODO : "Right" and "Left" doesn't mean much at this point ...
            {
                m_boatControl.sheetOut(boat);
                break;
            }
            case PlayerAction::Type::Left:
            {
                m_boatControl.sheetIn(boat);
                break;
            }
        }

        switch (action.rudderAction)
        {
            case PlayerAction::Type::Right:
            {
                m_boatControl.moveRudderRight(boat);
                break;
            }
            case PlayerAction::Type::Left:
            {
                m_boatControl.moveRudderLeft(boat);
                break;
            }
            case PlayerAction::Type::Center:
            {
                m_boatControl.centerRudder(boat);
            }
        }

        m_simulationMutex.unlock();
    }

    GameState Game::updateGame(gf::Time dt)
    {
        std::vector<BoatData> boatsData;

        m_simulationMutex.lock(); // TODO : is lock necessary for read only access?

        for (auto& p : m_players)
        {
            if (p.isConnected())
            {
                /*int updates = dt.asSeconds() / 0.0002;
                for (int i = 0; i < updates; i++) // TODO : 0.0002 -> 250 loops for dt=50ms, taking 0.16ms, per player
                { // TODO : JUST A TEST VERSION, shorter time spans are more likely to work with the sailing library
                    sailing_physics_update(p.getBoat(), m_fixedWind, 0.0002);
                }*/
                boatsData.push_back(p.getBoat().getBoatData());
            }
        }

        m_simulationMutex.unlock();

        return { boatsData };
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
        double physicGranularity = 0.00002;
        double loopsAmount = updateSec / physicGranularity; // TODO : some experimental things here

        while(m_simulationRunning) {
            clock.restart();
            m_simulationMutex.lock();

            for (int i=0; i<loopsAmount; i++) {
                for (auto& player : m_players)
                {
                    if (player.isConnected())
                    {
                        sailing_physics_update(player.getBoat(), m_fixedWind, physicGranularity);
                    }
                }
            }

            m_simulationMutex.unlock();
            double sleepTime = updateSec - clock.restart().asSeconds();

            //std::cout << sleepTime << "\n";
            assert(sleepTime >= 0); // Assert if the simulation is late compared to real world time (shouldn't be)
            // With granularity = 0.00002s, 50ms requires 2.500 loops, taking around 2-3ms per player in the game

            std::chrono::milliseconds span((int)(sleepTime * 1000));
            std::this_thread::sleep_for(span);
        }

    }

}
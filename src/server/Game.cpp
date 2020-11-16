//
// Created by augustin on 01/11/2020.
//

#include <iostream>

#include <gf/Random.h>

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
            case PlayerAction::Type::Right:
            {
                m_boatControl.pullSheet(boat);
                break;
            }
            case PlayerAction::Type::Left:
            {
                m_boatControl.releaseSheet(boat);
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
        }

        m_simulationMutex.unlock();
    }

    GameState Game::updateGame(gf::Time dt)
    {
        std::vector<BoatData> boatsData;

        m_simulationMutex.lock();

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
        m_simulationThread = std::thread(&Game::simulation, this);
    }

    void Game::simulation()
    {
        struct timespec t, t1;
        t.tv_sec  = 0;
        t.tv_nsec = 5000000L;

        for (;;) {
            while(m_simulationRunning) {
                m_simulationMutex.lock();

                for (int i=0; i<10000; i++) {
                    for (auto& player : m_players)
                    {
                        if (player.isConnected())
                        {
                            sailing_physics_update(player.getBoat(), m_fixedWind, 0.00002);
                        }
                    }
                }

                m_simulationMutex.unlock();
                nanosleep(&t, &t1);
            }
            nanosleep(&t, &t1);
        }
    }

}
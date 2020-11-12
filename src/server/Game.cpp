//
// Created by augustin on 01/11/2020.
//

#include <iostream>

#include <gf/Random.h>

#include "Game.h"

namespace sail
{

    Game::Game()
    : m_players()
    , m_started(false)
    , m_playersNb(0)
    {

    }

    void Game::start()
    {
        m_started = true;
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
        BoatWrapper& boat = player.getBoat();
        gf::Vector2f velocity = boat.getVelocity();
        /*switch (action.action) // TODO : to redefine
        {
            case PlayerAction::Type::Up:
            {
                velocity.y -= Speed;
                if (velocity.y < -MaxSpeed)
                    velocity.y = -MaxSpeed;
                break;
            }
            case PlayerAction::Type::Down:
            {
                velocity.y += Speed;
                if (velocity.y > MaxSpeed)
                    velocity.y = MaxSpeed;
                break;
            }
            case PlayerAction::Type::Right:
            {
                velocity.x += Speed;
                if (velocity.x > MaxSpeed)
                    velocity.x = MaxSpeed;
                break;
            }
            case PlayerAction::Type::Left:
            {
                velocity.x -= Speed;
                if (velocity.x < -MaxSpeed)
                    velocity.x = -MaxSpeed;
                break;
            }
        }*/
        boat.setVelocity(velocity);
    }

    GameState Game::updateGame(int dt)
    {
        std::vector<BoatData> boatsData;
        for (auto& p : m_players)
        {
            if (p.isConnected())
            {
                p.getBoat().update(dt);
                boatsData.push_back(p.getBoat().getBoatData());
            }
        }
        return { boatsData };
    }

}
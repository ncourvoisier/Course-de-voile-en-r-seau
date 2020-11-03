//
// Created by augustin on 01/11/2020.
//

#include <iostream>
#include "Game.h"

namespace sail
{

    Game::Game()
    : m_players()
    , m_started(false)
    {

    }

    void Game::start()
    {
        m_started = true;
    }

    std::vector<Player>& Game::getPlayers()
    {
        return m_players;
    }

    void Game::playerAction(Player& player, PlayerAction action)
    {
        if (! m_started)
            return;
        std::cout << "Action of : " << player.getName() << "\n";
        Boat& boat = player.getBoat();
        gf::Vector2f velocity = boat.getVelocity();
        switch (action.action)
        {
            case PlayerAction::Type::Up:
            {
                velocity.x += Speed;
                break;
            }
            case PlayerAction::Type::Down:
            {
                velocity.x -= Speed;
                break;
            }
            case PlayerAction::Type::Right:
            {
                velocity.y += Speed;
                break;
            }
            case PlayerAction::Type::Left:
            {
                velocity.y -= Speed;
                break;
            }
        }
        boat.setVelocity(velocity);
    }

    GameState Game::getGameState()
    {
        std::vector<BoatData> boatsData;
        for (auto& p : m_players)
        {
            if (p.isConnected())
                boatsData.push_back(p.getBoat().getBoatData());
        }
        return { boatsData };
    }

}
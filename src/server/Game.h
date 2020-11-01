#ifndef SAILINSANE_GAME_H
#define SAILINSANE_GAME_H


#include <vector>

#include "Player.h"

namespace sail
{

    class Game
    {
    public:
        Game();

        void start();

        std::vector<Player>& getPlayers();

        void playerAction(Player& player, PlayerAction action);

        GameState getGameState();

    private:
        static constexpr float Speed = 100.0f;

        std::vector<Player> m_players;

        bool m_started;
    };

}


#endif //SAILINSANE_GAME_H

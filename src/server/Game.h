#ifndef SAILINSANE_GAME_H
#define SAILINSANE_GAME_H


#include <vector>
#include <gf/Time.h>
#include <thread>

#include "Player.h"
#include "../physics/BoatController.h"
#include "ServerWorld.h"

namespace sail
{

    class Game
    {
    public:
        Game(int neededPlayers);

        void start();

        bool isStarted();

        bool connectPlayer(Player& player, gf::Id id, std::string userName);

        bool disconnectPlayer(Player& player);

        std::vector<Player>& getOnlinePlayers();
        std::vector<Player>& getOfflinePlayers();

        void playerAction(Player& player, PlayerAction& action);

        WorldData getWorldData();

        GameState update(gf::Time dt);

    private:
        void playerFinished(Player& player);

    private:
        int m_neededPlayers;
        int m_playersNb;

        ServerWorld m_world;

        BoatController m_boatController;
        std::vector<Player> m_onlinePlayers;

        std::vector<Player> m_offlinePlayers;

        bool m_started;

        Wind m_fixedWind;
    };

}


#endif //SAILINSANE_GAME_H

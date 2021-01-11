#ifndef SAILINSANE_GAME_H
#define SAILINSANE_GAME_H


#include <vector>
#include <gf/Time.h>
#include <thread>

#include "Player.h"
#include "../physics/BoatController.h"
#include "World.h"

namespace sail
{

    class Game
    {
    public:
        Game(int neededPlayers);

        void start();

        bool isStarted();

       // gf::Id addPlayer(gf::TcpSocket socket);

        bool connectPlayer(Player& player, gf::Id id, std::string userName);

        bool disconnectPlayer(Player& player);

        std::vector<Player>& getOnlinePlayers();

        void playerAction(Player& player, PlayerAction& action);

        WorldData getWorldData();

        GameState update(gf::Time dt);

    private:
        void runSimulation();

        void simulation();

        void checkCoordinates(Player& player);

    private:
        static constexpr float Speed = 25.0f;
        static constexpr float MaxSpeed = Speed * 4;

        int m_neededPlayers;

        World m_world;

        BoatController m_boatController;

        std::vector<Player> m_onlinePlayers;
        std::vector<Player> m_offlinePlayers;

        int m_playersNb;

        bool m_started;

        Wind m_fixedWind;

        std::thread m_simulationThread;

        std::mutex m_simulationMutex;

        std::atomic_bool m_simulationRunning;
    };

}


#endif //SAILINSANE_GAME_H

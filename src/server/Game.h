#ifndef SAILINSANE_GAME_H
#define SAILINSANE_GAME_H


#include <vector>
#include <gf/Time.h>
#include <thread>

#include "Player.h"
#include "../physics/BoatControl.h"

namespace sail
{

    class Game
    {
    public:
        Game();

        void start();

       // gf::Id addPlayer(gf::TcpSocket socket);

        bool connectPlayer(Player& player, gf::Id id, std::string userName);

        std::vector<Player>& getPlayers();

        void playerAction(Player& player, PlayerAction action);

        GameState updateGame(gf::Time dt);

    private:
        void runSimulation();

        void simulation();

    private:
        static constexpr int NeededPlayers = 2;

        static constexpr float Speed = 25.0f;
        static constexpr float MaxSpeed = Speed * 4;

        BoatControl m_boatControl;

        std::vector<Player> m_players;

        int m_playersNb;

        bool m_started;

        Wind m_fixedWind;

        std::thread m_simulationThread;

        std::mutex m_simulationMutex;

        std::atomic_bool m_simulationRunning;
    };

}


#endif //SAILINSANE_GAME_H

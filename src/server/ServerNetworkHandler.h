#ifndef SAILINSANE_SERVERNETWORKHANDLER_H
#define SAILINSANE_SERVERNETWORKHANDLER_H

#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>
#include <gf/Queue.h>
#include <thread>

#include "Player.h"
#include "Game.h"

namespace sail
{

    class ServerNetworkHandler
    {
    public:
        static constexpr int TicksPerSecond = 20;

        static constexpr int TickLength = 1000 / TicksPerSecond;

        ServerNetworkHandler(const std::string service);

        void broadcast(const gf::Packet& packet);

        void processPackets();

        void run();

        static void terminationHandler(int signum);

    private:
        Game m_game;

        gf::TcpListener m_listener;

        gf::SocketSelector m_selector;

        static std::atomic_bool g_running;
    };

}

#endif //SAILINSANE_SERVERNETWORKHANDLER_H

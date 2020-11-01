#ifndef SAILINSANE_SERVERNETWORKHANDLER_H
#define SAILINSANE_SERVERNETWORKHANDLER_H

#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>

#include "Player.h"
#include "Game.h"

namespace sail
{

    class ServerNetworkHandler
    {
    public:
        ServerNetworkHandler(const std::string service);

        void run();

        void broadcast(const gf::Packet& packet);

    private:
        Game m_game;

        gf::TcpListener m_listener;

        gf::SocketSelector m_selector;
    };

}

#endif //SAILINSANE_SERVERNETWORKHANDLER_H

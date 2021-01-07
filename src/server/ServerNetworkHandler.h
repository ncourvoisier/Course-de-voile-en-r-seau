#ifndef SAILINSANE_SERVERNETWORKHANDLER_H
#define SAILINSANE_SERVERNETWORKHANDLER_H

#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>
#include <gf/Queue.h>
#include <thread>
#include <gf/Message.h>

#include "Player.h"
#include "Game.h"

namespace sail
{

    class ServerNetworkHandler
    {
    public:
        ServerNetworkHandler(const std::string service, Game& game);

        void broadcast(const gf::Packet& packet);

        void receivePackets(gf::Time timeout);
        void processPackets();

        void sendPositions();

        gf::MessageStatus onPlayerDied(gf::Id id, gf::Message *msg);
        gf::MessageStatus onPlayerFinished(gf::Id id, gf::Message *msg);

    private:
        Game& m_game;

        gf::TcpListener m_listener;

        gf::SocketSelector m_selector;
    };

}

#endif //SAILINSANE_SERVERNETWORKHANDLER_H

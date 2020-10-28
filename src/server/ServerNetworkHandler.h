#ifndef SAILINSANE_SERVERNETWORKHANDLER_H
#define SAILINSANE_SERVERNETWORKHANDLER_H

#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>

#include "Player.h"

namespace sail
{

    class ServerNetworkHandler
    {
    public:
        ServerNetworkHandler(std::string service);
        void run();

    private:
        gf::TcpListener listener;
        gf::SocketSelector selector;
        std::vector<Player> players;
    };

}

#endif //SAILINSANE_SERVERNETWORKHANDLER_H

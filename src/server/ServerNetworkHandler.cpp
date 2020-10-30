//
// Created by augustin on 28/10/2020.
//

#include <gf/Packet.h>
#include <iostream>

#include "ServerNetworkHandler.h"
#include "../../Protocol.h"

namespace sail
{

    ServerNetworkHandler::ServerNetworkHandler(std::string service)
    : listener(service)
    {

    }

    void ServerNetworkHandler::run()
    {

        selector.addSocket(listener);
        for (;;)
        {
            if (selector.wait() == gf::SocketSelectorStatus::Event)
            {
                for (auto& player : players)
                {
                    auto& socket = player.getSocket();
                    //std::cout << "Searching for socket : " << socket << "\n";
                    assert(selector.isReady(socket));
                    if (selector.isReady(socket))
                    {
                        gf::Packet packet;
                        if (socket.recvPacket(packet) != gf::SocketStatus::Data)
                        {
                            // TODO : Remove closed sockets
                            continue;
                        }
                        switch (packet.getType())
                        {
                            case Connect::type:
                                Connect co{packet.as<Connect>()};
                                std::cout << "New user : " << co.username << "\n";
                                break;
                        }
                    }
                }
                if (selector.isReady(listener))
                {
                    // the listener is ready, accept a new connection
                    gf::TcpSocket socket = listener.accept();
                    Boat boat(gf::Vector2f(0,0)); // TODO : change departure position
                    Player player(std::move(socket), boat);
                    players.push_back(std::move(player));
                    selector.addSocket(players.back().getSocket());
                }
            }
        }

    }

}
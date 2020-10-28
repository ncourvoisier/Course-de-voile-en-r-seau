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
        // THIS VERSION IS NOT WORKING AND I HAVE NO SINGLE IDEA WHY HA HA :

        /*selector.addSocket(listener);
        for (;;)
        {
            if (selector.wait() == gf::SocketSelectorStatus::Event)
            {
                for (auto& player : players)
                {
                    auto& socket = player.getSocket();
                    std::cout << "Searching for socket : " << socket << "\n";
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
                    Player player(socket);
                    players.push_back(player);
                    selector.addSocket(player.getSocket());
                    std::cout << "Added socket : " << player.getSocket() << "\n";
                }
            }
        }*/

        // TEMPORARY VERSION :

        selector.addSocket(listener);
        std::vector<gf::TcpSocket> sockets;

        for (;;) {
            if (selector.wait() == gf::SocketSelectorStatus::Event) {
                for (auto& socket : sockets) {
                    if (selector.isReady(socket)) {
                        // read something on the socket
                        gf::Packet packet;
                        if (socket.recvPacket(packet) != gf::SocketStatus::Data)
                        {
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
                if (selector.isReady(listener)) {
                    // the listener is ready, accept a new connection
                    gf::TcpSocket socket = listener.accept();
                    sockets.push_back(std::move(socket));
                    selector.addSocket(sockets.back());
                }
            }
        }

    }

}
//
// Created by augustin on 28/10/2020.
//

#include <gf/Packet.h>
#include <gf/Random.h>
#include <iostream>

#include "ServerNetworkHandler.h"
#include "../Protocol.h"

namespace sail
{

    ServerNetworkHandler::ServerNetworkHandler(std::string service)
    : m_listener(service)
    {

    }

    void ServerNetworkHandler::broadcast(const gf::Packet& packet)
    {
        for (auto& player : m_players) {
            if (player.isConnected())
                player.getSocket().sendPacket(packet);
        }
    }

    void ServerNetworkHandler::run()
    {

        m_selector.addSocket(m_listener);
        for (;;)
        {
            if (m_selector.wait() == gf::SocketSelectorStatus::Event)
            {
                for (auto& player : m_players)
                {
                    auto& socket = player.getSocket();
                    //std::cout << "Searching for socket : " << socket << "\n";
                    assert(m_selector.isReady(socket));
                    if (m_selector.isReady(socket))
                    {
                        gf::Packet packet;
                        if (socket.recvPacket(packet) != gf::SocketStatus::Data)
                        {
                            // TODO : Remove closed sockets
                            continue;
                        }
                        switch (packet.getType())
                        {
                            case ClientGreeting::type:
                            {
                                ClientGreeting clientG{packet.as<ClientGreeting>()};
                                std::cout << "New user : " << clientG.username << "\n";
                                gf::Random rand;
                                gf::Id newId{rand.computeId()};
                                ServerGreeting serverG{newId};
                                gf::Packet serverGPacket;
                                serverGPacket.is<ServerGreeting>(serverG);
                                socket.sendPacket(serverGPacket);

                                gf::Packet broadcastPacket;
                                broadcastPacket.is<PlayerJoins>(sail::PlayerJoins{newId});
                                broadcast(broadcastPacket);

                                player.setConnected(true);
                                break;
                            }
                        }
                    }
                }
                if (m_selector.isReady(m_listener))
                {
                    // the listener is ready, accept a new connection
                    gf::TcpSocket socket = m_listener.accept();
                    Boat boat(gf::Vector2f(0,0)); // TODO : change departure position
                    Player player(std::move(socket), boat);
                    m_players.push_back(std::move(player));
                    m_selector.addSocket(m_players.back().getSocket());
                }
            }
        }

    }

}
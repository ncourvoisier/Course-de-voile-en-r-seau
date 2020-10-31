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
                            { // TODO : UGLY PART, to rewrite
                                ClientGreeting clientG {packet.as<ClientGreeting>()};
                                std::cout << "New user : " << clientG.username << "\n";
                                gf::Random rand;
                                gf::Id newId = rand.computeId();
                                std::vector<PlayerData> playersData;
                                for (auto& p : m_players) {
                                    if (p.isConnected())
                                        playersData.push_back(p.getPlayerData());
                                }
                                ServerGreeting serverG = { newId, playersData }; ////
                                gf::Packet serverGPacket;
                                serverGPacket.is(serverG);
                                socket.sendPacket(serverGPacket);

                                gf::Packet broadcastPacket;
                                broadcastPacket.is<PlayerJoins>(PlayerJoins{{newId, clientG.username}});
                                broadcast(broadcastPacket);

                                player.connect(newId, clientG.username);
                                break;
                            }
                        }
                    }
                }
                if (m_selector.isReady(m_listener))
                {
                    // the listener is ready, accept a new connection
                    gf::TcpSocket socket = m_listener.accept();
                    Player player(std::move(socket));
                    m_players.push_back(std::move(player));
                    m_selector.addSocket(m_players.back().getSocket());
                }
            }
        }

    }

}
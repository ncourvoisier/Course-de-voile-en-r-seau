//
// Created by augustin on 28/10/2020.
//

#include <gf/Packet.h>
#include <gf/Random.h>
#include <iostream>
#include <chrono>

#include "ServerNetworkHandler.h"
#include "../Protocol.h"

namespace sail
{

    ServerNetworkHandler::ServerNetworkHandler(std::string service)
    : m_listener(service)
    , m_game()
    {

    }

    void ServerNetworkHandler::broadcast(const gf::Packet& packet)
    {
        for (auto& player : m_game.getPlayers()) {
            if (player.isConnected())
                player.getSocket().sendPacket(packet);
        }
    }

    uint64_t sinceEpochMs()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    void ServerNetworkHandler::run()
    {
        int playerNb = 0; // TODO : temporary stuff, create a gf::GameConfig or something
        int neededPlayers = 2;

        m_selector.addSocket(m_listener);

        static constexpr gf::Time Timeout = gf::milliseconds(TickLength);

        for (;;)
        {
            //// SENDING DATAS ////
            GameState gs = m_game.getGameState();
            gf::Packet gsPacket;
            gsPacket.is(gs);
            broadcast(gsPacket);
            std::cout << "sending packets\n";
            ///////////////////////

            std::cout << "waiting packets\n";
            if (m_selector.wait(Timeout) == gf::SocketSelectorStatus::Event)
            {
                for (auto &player : m_game.getPlayers())
                {
                    auto &socket = player.getSocket();
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
                                ClientGreeting clientG{packet.as<ClientGreeting>()};
                                std::cout << "New user : " << clientG.username << "\n";
                                gf::Random rand;
                                gf::Id newId{rand.computeId()};
                                std::vector<PlayerData> playersData;
                                for (auto &p : m_game.getPlayers())
                                {
                                    if (p.isConnected())
                                        playersData.push_back(p.getPlayerData());
                                }
                                ServerGreeting serverG = {newId, playersData}; ////
                                gf::Packet serverGPacket;
                                serverGPacket.is(serverG);
                                socket.sendPacket(serverGPacket);

                                gf::Packet broadcastPacket;
                                broadcastPacket.is<PlayerJoins>(PlayerJoins{{newId, clientG.username}});
                                broadcast(broadcastPacket);

                                player.connect(newId, clientG.username);

                                if (++playerNb == neededPlayers)
                                { // TODO : temporary
                                    gf::Packet readyPacket;
                                    readyPacket.is(GameReady());
                                    broadcast(readyPacket);
                                    m_game.start();
                                }
                                break;
                            }
                            case ClientBoatData::type:
                            {
                                if (!player.isConnected())
                                    continue;
                                ClientBoatData boatData {packet.as<ClientBoatData>()};
                                player.getBoat().setVelocity(boatData.velocity);
                                player.getBoat().setPosition(boatData.position); // TODO : NO VALIDITY CHECKS AT ALL, JUST A TEST
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
                    m_game.getPlayers().push_back(std::move(player)); // TODO : change for an addPlayer function
                    m_selector.addSocket(m_game.getPlayers().back().getSocket());
                }
            }
        }
    }
}
//
// Created by augustin on 28/10/2020.
//

#include <gf/Packet.h>
#include <gf/Random.h>
#include <iostream>
#include <chrono>
#include <gf/Clock.h>

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

    void ServerNetworkHandler::processPackets()
    {
        for (Player& p : m_game.getPlayers())
        {
            int packetsNb = 0;
            gf::TcpSocket& socket = p.getSocket();
            auto& pendingQueue = p.getPendingPackets();
            gf::Packet packet;
            while (!pendingQueue.empty())
            {
                packetsNb++;
                packet = pendingQueue.front();
                pendingQueue.pop_front();
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
                            {
                                playersData.push_back(p.getPlayerData());
                                std::cout << "Player already here : " << p.getName() << "\n";
                            }
                        }
                        ServerGreeting serverG = {newId, playersData}; ////
                        gf::Packet serverGPacket;
                        serverGPacket.is(serverG);
                        socket.sendPacket(serverGPacket);

                        gf::Packet broadcastPacket;
                        broadcastPacket.is<PlayerJoins>(PlayerJoins{{newId, clientG.username}});
                        broadcast(broadcastPacket);

                        if (m_game.connectPlayer(p, newId, clientG.username))
                        {
                            gf::Packet readyPacket;
                            readyPacket.is(GameReady());
                            broadcast(readyPacket);
                        }
                        std::cout << "New player : " << p.getId() << " with name : " << p.getName() << "\n";
                        break;
                    }
                    case PlayerAction::type:
                    {
                        if (!p.isConnected())
                            continue;
                        PlayerAction action = packet.as<PlayerAction>();
                        m_game.playerAction(p, action);
                        break;
                    }
                }
            }
            if (packetsNb > 0)
                std::cout << packetsNb << " packets processed\n";
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

        gf::Clock clock;

        int packS = 0;

        for (;;)
        {
            processPackets();

            //// SENDING DATAS ////
            GameState gs = m_game.updateGame(clock.restart().asMilliseconds());
            gf::Packet gsPacket;
            gsPacket.is(gs);
            broadcast(gsPacket);
            std::cout << "sending packets " << ++packS << "\n";
            ///////////////////////

            //std::this_thread::sleep_for(std::chrono::milliseconds(TickLength)); // Sleep to save some processing time

            uint64_t timeNow = sinceEpochMs();
            while (sinceEpochMs() < timeNow + TickLength)
            {
                //std::cout << "waiting packets\n";
                if (m_selector.wait(Timeout) == gf::SocketSelectorStatus::Event)
                {
                    for (auto &player : m_game.getPlayers())
                    {
                        auto &socket = player.getSocket();
                        if (m_selector.isReady(socket))
                        {
                            gf::Packet packet;
                            gf::SocketStatus status = socket.recvPacket(packet);
                            if (status == gf::SocketStatus::Data)
                            {
                                std::cout << "Received packet\n"; // TODO : way too many packets are received here, why ?
                            }
                            else
                            {
                                // TODO : Remove closed sockets
                                continue;
                            }
                            player.getPendingPackets().push_back(packet);
                        }
                    }
                    if (m_selector.isReady(m_listener))
                    {
                        // the listener is ready, accept a new connection
                        gf::TcpSocket socket = m_listener.accept();
                        Player player(std::move(socket));
                        m_game.getPlayers().push_back(std::move(player)); // TODO : change for an addPlayer function
                        //gf::Id newId = m_game.addPlayer(std::move(socket));
                        m_selector.addSocket(m_game.getPlayers().back().getSocket());
                        std::cout << "Socket added to selector : " << m_game.getPlayers().back().getSocket() << "\n";
                    }
                }
            }
        }
    }
}
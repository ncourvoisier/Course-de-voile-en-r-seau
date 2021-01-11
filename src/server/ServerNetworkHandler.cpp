//
// Created by augustin on 28/10/2020.
//

#include <gf/Packet.h>
#include <gf/Random.h>
#include <iostream>
#include <csignal>
#include <chrono>
#include <gf/Clock.h>
#include <gf/Log.h>

#include "ServerNetworkHandler.h"
#include "../Protocol.h"
#include "../LoggingUtils.h"
#include "Managers.h"
#include "GameMessages.h"

namespace sail
{

    ServerNetworkHandler::ServerNetworkHandler(std::string service, Game& game)
    : m_listener(service)
    , m_selector()
    , m_game(game)
    {
        m_selector.addSocket(m_listener);

        gMessageManager().registerHandler<PlayerDied>(&ServerNetworkHandler::onPlayerDied, this);
        gMessageManager().registerHandler<PlayerFinished>(&ServerNetworkHandler::onPlayerFinished, this);
    }

    gf::MessageStatus ServerNetworkHandler::onPlayerDied(gf::Id id, gf::Message *msg)
    {
        assert(id == PlayerDied::type);
        auto died = static_cast<PlayerDied*>(msg);
        PlayerEvent death { died->id, PlayerEvent::EventType::Death };
        gf::Packet deathP;
        deathP.is<PlayerEvent>(death);
        broadcast(deathP); // TODO : sockets are currently binded to player objects, but they should only be located in the ServerNetworkHandler, in a map
        return gf::MessageStatus::Keep;
    }

    gf::MessageStatus ServerNetworkHandler::onPlayerFinished(gf::Id id, gf::Message *msg)
    {
        assert(id == PlayerFinished::type);
        auto finished = static_cast<PlayerFinished*>(msg);
        PlayerEvent finish { finished->id, PlayerEvent::EventType::Finish };
        gf::Packet finishP;
        finishP.is<PlayerEvent>(finish);
        broadcast(finishP);
        return gf::MessageStatus::Keep;
    }

    void ServerNetworkHandler::broadcast(const gf::Packet& packet)
    {
        for (auto& player : m_game.getOnlinePlayers())
        {
            if (player.isConnected())
            {
                //std::cout << "Before sending packet of size : " << packet.bytes.size() << " (type: " << humanizePacketType(packet.type) << ")\n";
                if (auto status = player.getSocket().sendPacket(packet); status != gf::SocketStatus::Data)
                {
                    gf::Log::error("Couldn't send packet to '%s'\n", player.getName().c_str());
                }
                //std::cout << "After sending\n";
            }
        }
    }

    void ServerNetworkHandler::processPackets()
    {
        for (Player& p : m_game.getOnlinePlayers())
        {
            int packetsNb = 0;
            gf::TcpSocket& socket = p.getSocket();
            auto& pendingQueue = p.getPendingPackets();
            gf::Packet packet;
            if (! pendingQueue.empty())
            {
                packetsNb++;
                packet = pendingQueue.front();
                pendingQueue.pop_front();
                switch (packet.getType())
                {
                    case ClientGreeting::type:
                    {
                        ClientGreeting clientG{packet.as<ClientGreeting>()};
                        std::cout << "New user : " << clientG.username << "\n";
                        gf::Random rand;
                        gf::Id newId{rand.computeId()};
                        std::vector<PlayerData> playersData;
                        for (auto &p : m_game.getOnlinePlayers())
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
                        std::cout << "Sent server greeting\n";

                        gf::Packet broadcastPacket;
                        broadcastPacket.is<PlayerJoins>(PlayerJoins{{newId, clientG.username}});
                        broadcast(broadcastPacket);
                        std::cout << "Sent player joined broadcats\n";

                        if (m_game.connectPlayer(p, newId, clientG.username))
                        {
                            std::cout << "Sending the world datas to players\n"; // REACHES HERE
                            gf::Packet worldPacket;
                            worldPacket.is(m_game.getWorldData());
                            std::cout << "Ready packet built\n";
                            broadcast(worldPacket);
                            m_game.start();
                            std::cout << "Starting the game\n"; // BUT NOT HERE
                        }
                        std::cout << "New player : " << p.getId() << " with name : " << p.getName() << "\n";
                        break;
                    }
                    case PlayerAction::type:
                    {
                        if (!p.isConnected())
                            continue;
                        auto action = packet.as<PlayerAction>();
                        m_game.playerAction(p, action);
                        break;
                    }
                }
            }
            /*if (packetsNb > 0)
                std::cout << packetsNb << " packets processed\n";*/
        }
    }

    void ServerNetworkHandler::receivePackets(gf::Time timeout)
    {
        if (m_selector.wait(timeout) == gf::SocketSelectorStatus::Event)
        {
            for (auto &player : m_game.getOnlinePlayers())
            {
                auto &socket = player.getSocket();
                if (m_selector.isReady(socket))
                {
                    gf::Packet packet;
                    gf::SocketStatus status = socket.recvPacket(packet);
                    if (status != gf::SocketStatus::Data)
                    {
                        disconnectPlayer(player);
                        continue;
                    }
                    player.getPendingPackets().push_back(packet);
                }
            }
            if (m_selector.isReady(m_listener))
            {
                std::cout << "Attempt to connect\n";
                // the listener is ready, accept a new connection
                gf::TcpSocket socket = m_listener.accept();
                Player player(std::move(socket));
                m_game.getOnlinePlayers().push_back(std::move(player));
                //gf::Id newId = m_game.addPlayer(std::move(socket));
                m_selector.addSocket(m_game.getOnlinePlayers().back().getSocket());
                std::cout << "Socket added to selector : " << m_game.getOnlinePlayers().back().getSocket() << "\n";
            }
        }
    }

    void ServerNetworkHandler::sendPositions(gf::Time dt)
    {
        GameState gs = m_game.update(dt);
        for (auto& player : m_game.getOnlinePlayers())
        {
            gs.lastAckActionId = player.getLastAckActionId();
            gf::Packet gsPacket;
            gsPacket.is(gs);
            player.getSocket().sendPacket(gsPacket);
        }
    }

    void ServerNetworkHandler::disconnectPlayer(Player& player)
    {
        gf::Log::warning("Player '%s' disconnected\n", player.getName().c_str());
        if (m_game.disconnectPlayer(player))
        {
            m_selector.removeSocket(player.getSocket());

            PlayerLeaves leave{player.getId()};
            gf::Packet leavePacket;
            leavePacket.is(leave);
            broadcast(leavePacket);
        }
    }

}
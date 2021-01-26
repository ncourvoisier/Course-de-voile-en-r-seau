#include <gf/Packet.h>
#include <gf/Random.h>
#include <gf/Clock.h>
#include <gf/Log.h>

#include "ServerNetworkHandler.h"
#include "../LoggingUtils.h"
#include "Managers.h"
#include "GameMessages.h"
#include "ServerStringConstants.h"

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
        broadcast(deathP);
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
                if (auto status = player.getSocket().sendPacket(packet); status != gf::SocketStatus::Data)
                {
                    gf::Log::error(ServerStringConstants::PacketNotSending, player.getName().c_str());
                }
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
            while (! pendingQueue.empty()) // If we use an "if" here : no roll-back will be ever felt by the client, BUT, the server
            {                              // will slowly become late as Ticks are being skipped in server overload condition, without any way those
                packetsNb++;               // those Ticks can be catch again
                packet = pendingQueue.front();
                pendingQueue.pop_front();
                switch (packet.getType())
                {
                    case ClientGreeting::type:
                    {
                        ClientGreeting clientG{packet.as<ClientGreeting>()};
                        gf::Log::info(ServerStringConstants::UserGreeting, clientG.username.c_str());
                        gf::Random rand;
                        gf::Id newId{rand.computeId()};
                        std::vector<PlayerData> playersData;
                        for (auto &po : m_game.getOnlinePlayers())
                        {
                            if (po.isConnected())
                            {
                                playersData.push_back(po.getPlayerData());
                            }
                        }
                        ServerGreeting serverG = {newId, playersData};
                        gf::Packet serverGPacket;
                        serverGPacket.is(serverG);
                        socket.sendPacket(serverGPacket);

                        gf::Packet broadcastPacket;
                        broadcastPacket.is<PlayerJoins>(PlayerJoins{{newId, clientG.username}});
                        broadcast(broadcastPacket);

                        if (m_game.connectPlayer(p, newId, clientG.username))
                        {
                            gf::Packet worldPacket;
                            worldPacket.is(m_game.getWorldData());
                            broadcast(worldPacket);

                            m_game.start();

                            GameStart start;
                            gf::Packet startPacket;
                            startPacket.is(start);
                            broadcast(startPacket);
                        }
                        gf::Log::info(ServerStringConstants::PlayerGivenId, p.getName().c_str(), p.getId());
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
                // the listener is ready, accept a new connection
                gf::TcpSocket socket = m_listener.accept();
                if (m_game.isStarted())
                {
                    gf::Packet p;
                    socket.recvPacket(p); // So the client can receive the GameRunning packet
                    GameRunning gr;
                    gf::Packet grPacket;
                    grPacket.is(gr);
                    socket.sendPacket(grPacket);
                    return;
                }
                Player player(std::move(socket));
                m_game.getOnlinePlayers().push_back(std::move(player));
                m_selector.addSocket(m_game.getOnlinePlayers().back().getSocket());
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
        gf::Log::info(ServerStringConstants::PlayerDisconnected, player.getName().c_str());
        if (m_game.disconnectPlayer(player))
        {
            Player& oldPlayer = m_game.getOfflinePlayers().back();
            m_selector.removeSocket(oldPlayer.getSocket());

            PlayerLeaves leave{oldPlayer.getId()};
            gf::Packet leavePacket;
            leavePacket.is(leave);
            broadcast(leavePacket);
        }
    }

}
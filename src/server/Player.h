#ifndef SAILINSANE_PLAYER_H
#define SAILINSANE_PLAYER_H

#include <gf/TcpSocket.h>
#include <gf/Id.h>
#include <gf/Packet.h>
#include <gf/Queue.h>

#include "ServerBoat.h"
#include "../Protocol.h"
#include "ServerWorld.h"

namespace sail
{

    class Player
    {
    public:
        Player(gf::TcpSocket);

        Player();

        inline bool operator==(const Player& other) { return m_id == other.getId(); }

        gf::TcpSocket& getSocket();

        ServerBoat& getBoat();

        bool isConnected() const;
        void connect(gf::Id id, std::string name, ServerWorld& world);

        void disconnect();

        gf::Id getId() const;

        const std::string& getName() const;

        PlayerData getPlayerData();

        std::deque<gf::Packet>& getPendingPackets();

        unsigned int getLastAckActionId();

        bool finished();
        void setFinished();

        void setLastAckActionId(unsigned int mLastAckActionId);

    private:
        gf::Id m_id;

        std::string m_name;

        gf::TcpSocket m_socket;

        std::deque<gf::Packet> m_pendingPackets;

        ServerBoat m_boat;

        bool m_connected;

        bool m_finished;

        unsigned int m_lastAckActionId;
    };

}

#endif //SAILINSANE_PLAYER_H

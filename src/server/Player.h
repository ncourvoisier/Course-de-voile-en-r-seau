#ifndef SAILINSANE_PLAYER_H
#define SAILINSANE_PLAYER_H

#include <gf/TcpSocket.h>
#include <gf/Id.h>
#include <gf/Packet.h>
#include <gf/Queue.h>

#include "BoatWrapper.h"
#include "../Protocol.h"

namespace sail
{

    class Player
    {
    public:
        Player(gf::TcpSocket);

        Player();

        gf::TcpSocket& getSocket();

        BoatWrapper& getBoat();

        bool isConnected() const;

        void connect(gf::Id id, std::string name);

        gf::Id getId() const;

        const std::string& getName() const;

        PlayerData getPlayerData();

        std::deque<gf::Packet>& getPendingPackets();

    private:
        gf::Id m_id;

        std::string m_name;

        gf::TcpSocket m_socket;

        std::deque<gf::Packet> m_pendingPackets;

        BoatWrapper m_boat;

        bool m_connected;
    };

}

#endif //SAILINSANE_PLAYER_H

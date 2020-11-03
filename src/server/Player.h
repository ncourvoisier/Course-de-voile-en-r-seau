#ifndef SAILINSANE_PLAYER_H
#define SAILINSANE_PLAYER_H

#include <gf/TcpSocket.h>
#include <gf/Id.h>
#include <gf/Packet.h>

#include "Boat.h"
#include "../Protocol.h"

namespace sail
{

    class Player
    {
    public:
        Player(gf::TcpSocket s);

        Player();

        gf::TcpSocket& getSocket();

        Boat& getBoat();

        bool isConnected() const;

        void connect(gf::Id id, std::string name);

        gf::Id getId() const;

        const std::string &getName() const;

        PlayerData getPlayerData();


    private:
        gf::Id m_id;

        std::string m_name;

        gf::TcpSocket m_socket;

        Boat m_boat;

        bool m_connected;
    };

}

#endif //SAILINSANE_PLAYER_H

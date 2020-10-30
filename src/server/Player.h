#ifndef SAILINSANE_PLAYER_H
#define SAILINSANE_PLAYER_H

#include <gf/TcpSocket.h>

#include "Boat.h"

namespace sail
{

    class Player
    {
    public:
        Player(gf::TcpSocket s, Boat boat);

        gf::TcpSocket& getSocket();

        Boat& getBoat();

        bool isConnected() const;

        void setConnected(bool connected);

    private:
        gf::TcpSocket m_socket;

        Boat m_boat;

        bool m_connected;
    };

}

#endif //SAILINSANE_PLAYER_H

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

    private:
        gf::TcpSocket m_socket;
        Boat m_boat;
    };

}

#endif //SAILINSANE_PLAYER_H

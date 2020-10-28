#ifndef SAILINSANE_PLAYER_H
#define SAILINSANE_PLAYER_H

#include <gf/TcpSocket.h>

namespace sail
{

    class Player
    {
    public:
        Player(gf::TcpSocket& s);

        gf::TcpSocket& getSocket();

    private:
        gf::TcpSocket& p_socket;
    };

}

#endif //SAILINSANE_PLAYER_H

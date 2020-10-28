#include "Player.h"

namespace sail {

    Player::Player(gf::TcpSocket& socket)
    : p_socket(socket)
    {

    }

    gf::TcpSocket& Player::getSocket()
    {
        return p_socket;
    }

}
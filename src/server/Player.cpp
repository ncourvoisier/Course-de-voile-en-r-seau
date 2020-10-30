#include "Player.h"

namespace sail {

    Player::Player(gf::TcpSocket socket, Boat boat)
    : m_socket(std::move(socket))
    , m_boat(boat)
    {

    }

    gf::TcpSocket& Player::getSocket()
    {
        return m_socket;
    }

    Boat& Player::getBoat()
    {
        return m_boat;
    }

}
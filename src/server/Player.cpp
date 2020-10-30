#include "Player.h"

namespace sail {

    Player::Player(gf::TcpSocket socket, Boat boat)
    : m_socket(std::move(socket))
    , m_boat(boat)
    , m_connected(false)
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

    bool Player::isConnected() const
    {
        return m_connected;
    }

    void Player::setConnected(bool connected)
    {
        m_connected = connected;
    }

}
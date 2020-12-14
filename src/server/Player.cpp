#include "Player.h"

namespace sail {

    Player::Player(gf::TcpSocket socket)
    : m_socket(std::move(socket))
    , m_boat()
    , m_connected(false)
    {

    }

    gf::TcpSocket& Player::getSocket()
    {
        return m_socket;
    }

    ServerBoat& Player::getBoat()
    {
        assert(m_connected);
        return m_boat;
    }

    bool Player::isConnected() const
    {
        return m_connected;
    }

    void Player::connect(gf::Id id, std::string name, World& world)
    {
        m_connected = true;
        m_id = id;
        m_name = name;
        m_boat = ServerBoat(id, world.getStartingPosition().x, world.getStartingPosition().y);
    }

    gf::Id Player::getId() const
    {
        assert(m_connected);
        return m_id;
    }

    const std::string &Player::getName() const
    {
        assert(m_connected);
        return m_name;
    }

    PlayerData Player::getPlayerData()
    {
        assert(m_connected);
        return { m_id, m_name };
    }

    std::deque<gf::Packet>& Player::getPendingPackets()
    {
        return m_pendingPackets;
    }

}
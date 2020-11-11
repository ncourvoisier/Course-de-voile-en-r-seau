#include "Player.h"

namespace sail {

    Player::Player(gf::TcpSocket socket)
    : m_socket(std::move(socket))
    , m_connected(false)
    {

    }

    gf::TcpSocket& Player::getSocket()
    {
        return m_socket;
    }

    BoatWrapper& Player::getBoat()
    {
        assert(m_connected);
        return m_boat;
    }

    bool Player::isConnected() const
    {
        return m_connected;
    }

    void Player::connect(gf::Id id, std::string name)
    {
        m_connected = true;
        m_id = id;
        m_name = name;
        m_boat = BoatWrapper(m_id, {0, 0}); // TODO : change this
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
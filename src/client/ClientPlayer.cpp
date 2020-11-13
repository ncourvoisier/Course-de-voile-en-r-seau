#include <gf/Color.h>
#include "ClientPlayer.h"

namespace sail
{

    ClientPlayer::ClientPlayer(gf::Id id, std::string userName)
    : m_id(id)
    , m_userName(userName)
    , m_boat(gf::Color::Green)
    {

    }

    gf::Id ClientPlayer::getId() const
    {
        return m_id;
    }

    const std::string &ClientPlayer::getUserName() const
    {
        return m_userName;
    }

    ClientBoat& ClientPlayer::getBoat()
    {
        return m_boat;
    }

}
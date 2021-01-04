#include <gf/Color.h>
#include "ClientPlayer.h"

namespace sail
{

    ClientPlayer::ClientPlayer(gf::Id id, std::string userName)
    : m_id(id)
    , m_boat(userName)
    {

    }

    gf::Id ClientPlayer::getId() const
    {
        return m_id;
    }

    ClientBoat& ClientPlayer::getBoat()
    {
        return m_boat;
    }

}
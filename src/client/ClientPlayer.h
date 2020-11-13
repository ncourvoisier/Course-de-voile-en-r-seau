//
// Created by augustin on 02/11/2020.
//

#ifndef SAILINSANE_CLIENTPLAYER_H
#define SAILINSANE_CLIENTPLAYER_H


#include <gf/Id.h>
#include "ClientBoat.h"

namespace sail
{

    class ClientPlayer
    {
    public:
        ClientPlayer(gf::Id id, std::string userName);

        gf::Id getId() const;

        const std::string &getUserName() const;

        ClientBoat &getBoat();

    private:
        gf::Id m_id;

        std::string m_userName;

        ClientBoat m_boat;
    };

}

#endif //SAILINSANE_CLIENTPLAYER_H

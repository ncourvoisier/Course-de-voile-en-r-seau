#ifndef SAILINSANE_GAMEMESSAGES_H
#define SAILINSANE_GAMEMESSAGES_H

#include <gf/Message.h>
#include "Player.h"

using namespace gf::literals;

namespace sail
{

    struct PlayerDied: public gf::Message
    {
        static constexpr gf::Id type = "PlayerDied"_id;

        gf::Id id;
    };

    struct PlayerFinished: public gf::Message
    {
        static constexpr gf::Id type = "PlayerFinished"_id;

        gf::Id id;
    };

}

#endif //SAILINSANE_GAMEMESSAGES_H

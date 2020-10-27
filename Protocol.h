#ifndef SAILINSANE_PROTOCOL_H
#define SAILINSANE_PROTOCOL_H

#include <gf/Id.h>

using namespace gf::literals;

namespace sail {

    struct Boat {
        static constexpr gf::Id type = "Boat"_id;
        long int pos_x;
        long int pos_y;
        double rotation;
        double speed;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, Boat& data) {
        return ar | data.pos_x | data.pos_y | data.rotation | data.speed;
    }

}

#endif //SAILINSANE_PROTOCOL_H

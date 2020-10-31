#ifndef SAILINSANE_PROTOCOL_H
#define SAILINSANE_PROTOCOL_H

#include <gf/Id.h>
#include <vector>
#include <gf/Vector.h>

using namespace gf::literals;

namespace sail {

    struct BoatData
    {
        static constexpr gf::Id type = "BoatData"_id;
        gf::Id playerId;
        gf::Vector2f position;
        gf::Vector2f velocity;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, BoatData& data)
    {
        return ar | data.playerId | data.position | data.velocity;
    }

    struct PlayerData
    {
        static constexpr gf::Id type = "PlayerData"_id;
        gf::Id id;
        std::string name;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerData& data)
    {
        return ar | data.id | data.name;
    }

    struct GameStatus
    {
        static constexpr gf::Id type = "GameStatus"_id;
        std::vector<BoatData> boats;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, GameStatus& data)
    {
        return ar | data.boats;
    }

    struct ClientGreeting
    {
        static constexpr gf::Id type = "ClientGreeting"_id;
        std::string username;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientGreeting& data)
    {
        return ar | data.username;
    }

    struct ServerGreeting
    {
        static constexpr gf::Id type = "ServerGreeting"_id;
        gf::Id playerId;
        std::vector<PlayerData> players;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ServerGreeting& data)
    {
        return ar | data.playerId | data.players;
    }

    struct PlayerJoins
    {
        static constexpr gf::Id type = "PlayerJoins"_id;
        PlayerData player;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerJoins& data)
    {
        return ar | data.player;
    }

}

#endif //SAILINSANE_PROTOCOL_H

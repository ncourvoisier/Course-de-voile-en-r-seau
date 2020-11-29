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
        double xPos;
        double yPos;
        double angle;
        double sailAngle;
        double rudderAngle;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, BoatData& data)
    {
        return ar | data.playerId | data.xPos | data.yPos | data.angle | data.sailAngle | data.rudderAngle;
    }

    struct ClientBoatData
    {
        static constexpr gf::Id type = "ClientBoatData"_id;
        gf::Vector2f position;
        gf::Vector2f velocity;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, ClientBoatData& data)
    {
        return ar | data.position | data.velocity;
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

    struct GameState
    {
        static constexpr gf::Id type = "GameState"_id;
        std::vector<BoatData> boats;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, GameState& data)
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

    struct GameReady
    {
        static constexpr gf::Id type = "GameReady"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, GameReady& data)
    {
        return ar;
    }

    struct PlayerAction
    {
        static constexpr gf::Id type = "PlayerAction"_id;

        enum Type : uint16_t
        { // TODO : replace with actual sailing boat controls
            Right,
            Left,
            Center,
            None
        };

        Type sailAction = None;
        Type rudderAction = None;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerAction& data) {
        return ar | data.sailAction | data.rudderAction;
    }

}

#endif //SAILINSANE_PROTOCOL_H

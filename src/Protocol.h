#ifndef SAILINSANE_PROTOCOL_H
#define SAILINSANE_PROTOCOL_H

#include <gf/Id.h>
#include <vector>
#include <gf/Vector.h>
#include <gf/Array2D.h>

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

        double sheetLength;
        double rotationalVelocity;
        double velocity;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, BoatData& data)
    {
        return ar | data.playerId | data.xPos | data.yPos | data.angle | data.sailAngle | data.rudderAngle
                | data.sheetLength | data.rotationalVelocity | data.velocity;
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

    struct PlayerEvent
    {
        static constexpr gf::Id type = "PlayerEvent"_id;

        enum EventType : uint16_t
        {
            Death,
            Finish,
            Checkpoint,
            Unknown
        };

        gf::Id id;
        EventType event = Unknown;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerEvent& data)
    {
        return ar | data.id | data.event;
    }

    struct GameState
    {
        static constexpr gf::Id type = "GameState"_id;
        std::vector<BoatData> boats;
        unsigned int lastAckActionId;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, GameState& data)
    {
        return ar | data.boats | data.lastAckActionId;
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

    struct PlayerLeaves
    {
        static constexpr gf::Id type = "PlayerLeaves"_id;
        gf::Id id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerLeaves& data)
    {
        return ar | data.id;
    }

    struct WorldData
    {
        static constexpr gf::Id type = "WorldData"_id;
        gf::Array2D<float> terrain;
        gf::Array2D<float> windDirection;
        gf::Array2D<float> windSpeed;
        gf::Vector2d startingPosition;
        gf::Vector2d endingPosition;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, WorldData& data)
    {
        return ar | data.terrain | data.windDirection | data.windSpeed | data.startingPosition | data.endingPosition;
    }

    struct PlayerAction
    {
        static constexpr gf::Id type = "PlayerAction"_id;

        enum Type : uint16_t
        {
            Right,
            Left,
            Center,
            None
        };

        Type sailAction = None;
        Type rudderAction = None;

        unsigned int id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PlayerAction& data) {
        return ar | data.sailAction | data.rudderAction | data.id;
    }

    struct GameStart
    {
        static constexpr gf::Id type = "GameStart"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, GameStart& data) {
        return ar;
    }

    struct GameRunning
    {
        static constexpr gf::Id type = "GameRunning"_id;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, GameRunning& data) {
        return ar;
    }

}

#endif //SAILINSANE_PROTOCOL_H

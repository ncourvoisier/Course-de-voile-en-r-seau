#ifndef SAILINSANE_LOGGINGUTILS_H
#define SAILINSANE_LOGGINGUTILS_H

#include <ostream>
#include <gf/Socket.h>

namespace sail
{

    std::ostream& operator<<(std::ostream& os, gf::SocketStatus status)
    {
        switch (status)
        {
            case gf::SocketStatus::Data: return os << "Data";
            case gf::SocketStatus::Block: return os << "Block";
            case gf::SocketStatus::Close: return os << "Close";
            case gf::SocketStatus::Error: return os << "Error";
        }
    }

    const char* humanizePacketType(const gf::Id type)
    {
        switch (type)
        {
            case "BoatData"_id: return "BoatData";
            case "PlayerData"_id: return "PlayerData";
            case "WindData"_id: return "WindData";
            case "GameState"_id: return "GameState";
            case "ClientGreeting"_id: return "ClientGreeting";
            case "ServerGreeting"_id: return "ServerGreeting";
            case "PlayerJoins"_id: return "PlayerJoins";
            case "WorldData"_id: return "WorldData";
            case "PlayerAction"_id: return "PlayerAction";
            case "PlayerEvent"_id: return "PlayerEvent";
        }
        return "Unknown";
    }

}

#endif //SAILINSANE_LOGGINGUTILS_H

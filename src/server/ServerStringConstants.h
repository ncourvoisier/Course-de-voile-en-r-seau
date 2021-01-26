#ifndef SAILINSANE_SERVERSTRINGCONSTANTS_H
#define SAILINSANE_SERVERSTRINGCONSTANTS_H

namespace sail
{

    struct ServerStringConstants
    {

        static constexpr auto GameStarted = "Game started on port %s: waiting for %d players\n";
        static constexpr auto ServerLate = "Server running %d Ticks / %d Ms after real time\n";
        static constexpr auto ServerClosing = "Closing the server.\n";
        static constexpr auto PacketNotSending = "Couldn't send packet to '%s'\n";
        static constexpr auto UserGreeting = "User greeting: '%s'\n";
        static constexpr auto PlayerGivenId = "Player '%s' was given ID: %lu\n";
        static constexpr auto PlayerDisconnected = "Player '%s' disconnected\n";

    };

}

#endif //SAILINSANE_SERVERSTRINGCONSTANTS_H

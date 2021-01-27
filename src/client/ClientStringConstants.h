#ifndef SAILINSANE_CLIENTSTRINGCONSTANTS_H
#define SAILINSANE_CLIENTSTRINGCONSTANTS_H

namespace sail
{

    struct ClientStringConstants
    {

        static constexpr auto DisconnectedFromServer = "Disconnected from server\n";
        static constexpr auto LostConnection = "Lost connection with server\n";
        static constexpr auto PacketsNotSending = "Couldn't send packets to server\n";
        static constexpr auto GameAlreadyRunning = "A game is already running on this server\n";
        static constexpr auto InvalidServerGreeting = "Invalid server greeting received\n";
        static constexpr auto GreetingId = "Server sent ID: %lu\n";
        static constexpr auto PlayerAlreadyHere = "Player already here: '%s'\n";
        static constexpr auto PlayerConnected = "Opponent connected: '%s'\n";
        static constexpr auto UnexpectedPacketType = "Unexpected packet type received from server\n";
        static constexpr auto GameReady = "The game is ready\n";
        static constexpr auto UnknownPlayerEvent = "Received unknown PlayerEvent type\n";

    };

}

#endif //SAILINSANE_CLIENTSTRINGCONSTANTS_H

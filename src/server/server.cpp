#include <gf/Packet.h>
#include <iostream>
#include <gf/Clock.h>
#include <gf/Singleton.h>
#include <gf/MessageManager.h>
#include <csignal>
#include "ServerNetworkHandler.h"

#include "Managers.h"

void printUsage(char* execName)
{
    std::cout << "Usage: " << execName << " [port] [players number (2-10)]\n";
}

std::atomic_bool running(true);

void terminationHandler(int signum)
{
    running = false;
}

uint64_t sinceEpochMs()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(int argc, char* argv[])
{

    /// Arguments ///

    if (argc != 3)
    {
        printUsage(argv[0]);
        exit(1);
    }

    int playersNb = std::stoi(argv[2]);

    if (playersNb < 2 || playersNb > 10)
    {
        printUsage(argv[0]);
        exit(2);
    }

    /// Instances initialization ///

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(sail::gMessageManager);

    sail::Game game(playersNb);

    std::cout << "Game started on port " << argv[1] << " : waiting for " << playersNb << " players\n";

    sail::ServerNetworkHandler networkHandler(argv[1], game);

    std::signal(SIGINT, &terminationHandler);
    std::signal(SIGTERM, &terminationHandler);

    /// Waiting for players ///

    while (! game.isStarted())
    {
        networkHandler.receivePackets(gf::Time::Zero);
        networkHandler.processPackets();
    }

    /// Game loop ///

    static constexpr int TicksPerSecond = 20;
    static constexpr int TickLength = 1000 / TicksPerSecond;
    static constexpr gf::Time Timeout = gf::milliseconds(TickLength);

    while (running)
    {
        networkHandler.processPackets();
        networkHandler.sendPositions();

        uint64_t timeNow = sinceEpochMs();
        while (sinceEpochMs() < timeNow + TickLength)
        {
            networkHandler.receivePackets(Timeout);
        }
    }

    std::cout << "Closing the server.\n";
}

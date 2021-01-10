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

std::sig_atomic_t running(true);

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

    std::signal(SIGINT, terminationHandler); // 'Ctrl-C'
    std::signal(SIGQUIT, terminationHandler); // 'Ctrl-\'
    std::signal(SIGTSTP, terminationHandler);  // 'Ctrl-Z'

    /// Waiting for players ///

    while (! game.isStarted() && running)
    {
        networkHandler.receivePackets(gf::Time::Zero);
        networkHandler.processPackets();
    }

    /// Game loop ///

    static constexpr int TicksPerSecond = 20;
    static constexpr int TickLength = 1000 / TicksPerSecond;
    static constexpr gf::Time Timeout = gf::milliseconds(TickLength);

    gf::Clock clock;

    while (running)
    {
        uint64_t timeNow = sinceEpochMs();

        networkHandler.processPackets();
        networkHandler.sendPositions(Timeout); // TODO : fixed or variable DT here?

        while (sinceEpochMs() < timeNow + TickLength)
        {
            networkHandler.receivePackets(gf::Time::Zero);
        }
    }

    std::cout << "Closing the server.\n";
}

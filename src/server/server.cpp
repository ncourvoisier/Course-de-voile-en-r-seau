#include <iostream>
#include <gf/Clock.h>
#include <gf/Singleton.h>
#include <gf/Log.h>
#include <gf/MessageManager.h>
#include <csignal>
#include "ServerNetworkHandler.h"

#include "Managers.h"
#include "../Constants.h"

void printUsage(char* execName)
{
    std::cout << "Usage: " << execName << " [port] [players number (2-10)]\n";
}

std::sig_atomic_t running(true);

void terminationHandler(int signum)
{
    running = false;
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

    gf::Clock clock;
    gf::Time nextFrameTime = gf::Time::Zero;

    while (game.isStarted() && running)
    {
        nextFrameTime += sail::FrameTime;

        networkHandler.receivePackets(gf::Time::Zero);

        networkHandler.processPackets();
        networkHandler.sendPositions(sail::FrameTime);

        if (clock.getElapsedTime() >= nextFrameTime)
        {
            auto criticalTime = (clock.getElapsedTime() - nextFrameTime).asMilliseconds();
            gf::Log::warning("Server running %d Ticks / %d Ms after real time\n",
                             (criticalTime / sail::FrameTime.asMilliseconds()), criticalTime);
            continue;
        }

        std::chrono::milliseconds span((nextFrameTime - clock.getElapsedTime()).asMilliseconds());
        std::this_thread::sleep_for(span);
    }

    std::cout << "Closing the server.\n";
}

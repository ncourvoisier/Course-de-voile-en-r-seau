#include <gf/Packet.h>
#include <iostream>
#include <gf/Clock.h>
#include <gf/Singleton.h>
#include <gf/MessageManager.h>
#include "../Protocol.h"
#include "ServerNetworkHandler.h"

#include "../physics/Boat.h"
#include "../physics/Physics.h"
#include "Managers.h"

void printUsage(char* execName)
{
    std::cout << "Usage: " << execName << " [port] [players number (2-10)]\n";
}

int main(int argc, char* argv[])
{
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

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(sail::gMessageManager);

    sail::Game game(playersNb);

    std::cout << "Game started on port " << argv[1] << " : waiting for " << playersNb << " players\n";

    sail::ServerNetworkHandler networkHandler(argv[1], game);
    networkHandler.run();
}

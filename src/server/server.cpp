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

int main() {

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(sail::gMessageManager);

    sail::Game game;

    std::cout << "OK\n";

    sail::ServerNetworkHandler networkHandler("24680", game);
    networkHandler.run();

}

#include <gf/Packet.h>
#include <iostream>
#include <gf/Clock.h>
#include "../Protocol.h"
#include "ServerNetworkHandler.h"

#include "../physics/Boat.h"
#include "../physics/Physics.h"

int main() {

    sail::ServerNetworkHandler networkHandler("24680");
    networkHandler.run();

}

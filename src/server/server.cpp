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

    /*int n = 0.05 / 0.00002;
    std::cout << "loops required : " << n << "\n";
    sail::Boat boat;
    sail::Wind wind;
    gf::Clock clock;
    for (int i = 0; i < n; i++)
    {
        sail::sailing_physics_update(boat, wind, 0.00002);
    }
    std::cout << "Time taken to simulate : " << clock.restart().asMilliseconds() << "ms \n";*/
}

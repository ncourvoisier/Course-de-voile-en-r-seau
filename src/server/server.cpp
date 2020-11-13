#include <gf/Packet.h>
#include <iostream>
#include <gf/Clock.h>
#include "../Protocol.h"
#include "ServerNetworkHandler.h"

#include "../physics/Boat.h"
#include "../physics/Physics.h"

void dispBoat(sail::Boat& boat)
{
    std::cout << boat.getLatitude() << ", " << boat.getLongitude() << "\n";
}

int main() {

    sail::ServerNetworkHandler networkHandler("24680");
    networkHandler.run();

    /*sail::Wind wind;
    wind.setSpeed(2000);
    sail::Boat boat;
    dispBoat(boat);

    gf::Clock clock;

    double dt = 0.05;
    int update = dt / 0.0002;

    for (int i = 0; i < update; i++)
    {
        //sleep(1);
        sail::sailing_physics_update(boat, wind, 0.0002);
    }

    std::cout << "Time taken : " << clock.restart().asMilliseconds() << "\n";

    // 1000000 x 0.000002 = 2

    std::cout << "Speed : " << boat.getVelocity() << "\n";

    dispBoat(boat);*/

}

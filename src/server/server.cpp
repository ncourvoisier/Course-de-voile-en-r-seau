#include <gf/Packet.h>
#include "../Protocol.h"
#include "ServerNetworkHandler.h"

int main() {

    sail::ServerNetworkHandler networkHandler("24680");
    networkHandler.run();

}
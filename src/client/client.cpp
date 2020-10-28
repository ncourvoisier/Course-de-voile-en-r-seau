#include "../../Protocol.h"
#include "ClientNetworkHandler.h"

int main(int argc, char** argv) {

    sail::ClientNetworkHandler clientHandler;
    clientHandler.connect("127.0.0.1", "24680");

    sail::Connect co;
    if (argc > 1)
        co.username = argv[0];
    else
        co.username = "user";

    clientHandler.send<sail::Connect>(co);

}
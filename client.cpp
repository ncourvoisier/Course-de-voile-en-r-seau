#include <gf/TcpSocket.h>
#include <gf/Packet.h>
#include <iostream>
#include "Protocol.h"

int main(int argc, char** argv) {

    gf::TcpSocket socket("127.0.0.1", "24680");

    sail::Connect co;
    if (argc > 1)
        co.username = argv[0];
    else
        co.username = "user";

    gf::Packet coPacket;
    coPacket.is(co);

    if (socket.sendPacket(coPacket) != gf::SocketStatus::Data) {
        std::cout << "No data were sent\n";
    }

}
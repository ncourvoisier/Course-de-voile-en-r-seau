#include "ClientNetworkHandler.h"

namespace sail
{

    ClientNetworkHandler::ClientNetworkHandler()
    : is_connected(false)
    {

    }

    void ClientNetworkHandler::connect(const std::string& hostname,
                                       const std::string& service)
    {
        socket = gf::TcpSocket(hostname, service);
        is_connected = false || socket;
    }

    gf::Packet ClientNetworkHandler::receive()
    {
        gf::Packet packet;
        socket.recvPacket(packet);
        return packet;
    }

    bool ClientNetworkHandler::isConnected() { return is_connected; }

}
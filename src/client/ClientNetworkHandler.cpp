#include "ClientNetworkHandler.h"

namespace sail
{

    ClientNetworkHandler::ClientNetworkHandler()
    : m_connected(false)
    {

    }

    void ClientNetworkHandler::connect(const std::string& hostname,
                                       const std::string& service)
    {
        m_socket = gf::TcpSocket(hostname, service);
        m_connected = false || socket;
        m_socket.setNonBlocking(); // TODO : just an idea
    }

    gf::SocketStatus ClientNetworkHandler::receive(gf::Packet packet)
    {
        return m_socket.recvPacket(packet);
    }

    bool ClientNetworkHandler::isConnected() { return m_connected; }

    void ClientNetworkHandler::setBlocking()
    {
        m_socket.setBlocking();
    }

    void ClientNetworkHandler::setNonBlocking()
    {
        m_socket.setNonBlocking();
    }

}
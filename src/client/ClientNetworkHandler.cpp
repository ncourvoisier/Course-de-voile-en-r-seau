#include "ClientNetworkHandler.h"
#include "../Protocol.h"

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
        //m_socket.setNonBlocking(); // TODO : just an idea
        m_socket.setBlocking();
    }

    gf::SocketStatus ClientNetworkHandler::receive(gf::Packet& packet)
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

    gf::TcpSocket &ClientNetworkHandler::getSocket()
    {
        return m_socket;
    }

    void ClientNetworkHandler::packetHandling()
    {
        for (;;)
        {
            gf::Packet packet;
            gf::SocketStatus status = receive(packet);
            if (status != gf::SocketStatus::Data)
                exit(2);
            queue.push(std::move(packet));
        }
    }

    void ClientNetworkHandler::run()
    {
        thread = std::thread(&ClientNetworkHandler::packetHandling, this);
    }

    gf::Queue<gf::Packet> &ClientNetworkHandler::getQueue()
    {
        return queue;
    }

}
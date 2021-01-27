#include <gf/Log.h>
#include "ClientNetworkHandler.h"
#include "ClientStringConstants.h"

namespace sail
{

    ClientNetworkHandler::ClientNetworkHandler()
    : m_connected(false)
    , m_running(false)
    {

    }

    void ClientNetworkHandler::connect(const std::string& hostname,
                                       const std::string& service)
    {
        m_socket = gf::TcpSocket(hostname, service);
        m_connected = false || m_socket;
    }

    gf::SocketStatus ClientNetworkHandler::receive(gf::Packet& packet)
    {
        gf::SocketStatus status = m_socket.recvPacket(packet);
        if (status != gf::SocketStatus::Data)
        {
            gf::Log::error(ClientStringConstants::PacketsNotReceiving);
            close();
            exit(2);
        }
        return status;
    }

    void ClientNetworkHandler::packetHandling()
    {
        while (m_running)
        {
            gf::Packet packet;
            receive(packet);
            queue.push(std::move(packet));
        }
    }

    void ClientNetworkHandler::run()
    {
        m_running = true;
        m_thread = std::thread(&ClientNetworkHandler::packetHandling, this);
        m_thread.detach();
    }

    void ClientNetworkHandler::close()
    {
        m_running = false;
    }

    gf::Queue<gf::Packet> &ClientNetworkHandler::getQueue()
    {
        return queue;
    }

}
#ifndef SAILINSANE_CLIENTNETWORKHANDLER_H
#define SAILINSANE_CLIENTNETWORKHANDLER_H

#include <string>

#include <gf/Packet.h>
#include <gf/TcpSocket.h>
#include <gf/Queue.h>
#include <thread>

namespace sail
{

    class ClientNetworkHandler
    {
    public:
        ClientNetworkHandler();

        void connect(const std::string& hostname,
                const std::string& service);

        bool isConnected();

        void run();

        void packetHandling();

        gf::SocketStatus receive(gf::Packet& packet);

        template <typename T>
        gf::SocketStatus send(const T& data)
        {
            gf::Packet packet;
            packet.is(data);
            return m_socket.sendPacket(packet);
        }

        gf::Queue<gf::Packet>& getQueue();

    private:
        gf::TcpSocket m_socket;

        bool m_connected;

        std::thread m_thread;

        gf::Queue<gf::Packet> queue;
    };

}

#endif //SAILINSANE_CLIENTNETWORKHANDLER_H
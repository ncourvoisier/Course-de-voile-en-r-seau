#ifndef SAILINSANE_CLIENTNETWORKHANDLER_H
#define SAILINSANE_CLIENTNETWORKHANDLER_H

#include <string>

#include <gf/Packet.h>
#include <gf/TcpSocket.h>

namespace sail
{

    class ClientNetworkHandler
    {
    public:
        ClientNetworkHandler();

        void connect(const std::string& hostname,
                const std::string& service);

        bool isConnected();

        gf::SocketStatus receive(gf::Packet packet);

        template <typename T>
        void send(const T& data)
        {
            gf::Packet packet;
            packet.is(data);
            m_socket.sendPacket(packet); // TODO : handle errors
        }

        void setBlocking();

        void setNonBlocking();

    private:
        gf::TcpSocket m_socket;

        bool m_connected;
    };

}

#endif //SAILINSANE_CLIENTNETWORKHANDLER_H
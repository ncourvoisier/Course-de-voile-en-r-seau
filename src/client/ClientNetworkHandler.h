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

        template <typename T>
        void send(const T& data)
        {
            gf::Packet packet;
            packet.is(data);
            socket.sendPacket(packet); // TODO : handle errors
        }
        gf::Packet receive();

    private:
        gf::TcpSocket socket;
        bool is_connected;
    };

}

#endif //SAILINSANE_CLIENTNETWORKHANDLER_H
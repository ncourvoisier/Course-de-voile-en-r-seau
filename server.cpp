#include <gf/TcpListener.h>
#include <gf/SocketSelector.h>
#include <gf/Packet.h>
#include <iostream>
#include "Protocol.h"

int main() {

    // players names
    std::vector<std::string> user_names;

    gf::TcpListener listener("24680");
    gf::SocketSelector selector;
    selector.addSocket(listener);
    std::vector<gf::TcpSocket> sockets;

    for (;;) {
        if (selector.wait() == gf::SocketSelectorStatus::Event) {
            for (auto it=sockets.begin(); it<sockets.end(); it++) {
                auto& socket = *it;
                if (selector.isReady(socket)) {
                    // read something on the socket
                    gf::Packet packet;
                    if (socket.recvPacket(packet) != gf::SocketStatus::Data) {
                        std::cout << "Disconnecting socket : " << socket << "\n";
                        sockets.erase(it);
                        continue;
                    }
                    if (packet.getType() == sail::Connect::type) {
                        // a player is connecting
                        auto co {packet.as<sail::Connect>()};
                        user_names.push_back(co.username);
                        std::cout << "New player : " << co.username << "\n";
                    }
                }
            }
            if (selector.isReady(listener)) {
                // the listener is ready, accept a new connection
                gf::TcpSocket socket = listener.accept();
                sockets.push_back(std::move(socket));
                selector.addSocket(sockets.back());
            }
        }
    }

}
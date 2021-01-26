#ifndef SAILINSANE_LOGGINGUTILS_H
#define SAILINSANE_LOGGINGUTILS_H

#include <ostream>
#include <gf/Socket.h>

namespace sail
{

    std::ostream& operator<<(std::ostream& os, gf::SocketStatus status)
    {
        switch (status)
        {
            case gf::SocketStatus::Data: return os << "Data";
            case gf::SocketStatus::Block: return os << "Block";
            case gf::SocketStatus::Close: return os << "Close";
            case gf::SocketStatus::Error: return os << "Error";
            default:
                assert(false);
        }
    }

}

#endif //SAILINSANE_LOGGINGUTILS_H

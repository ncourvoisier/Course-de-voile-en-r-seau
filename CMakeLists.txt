cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SAILINSANE
  VERSION 0.1.0
)

find_package(gf REQUIRED COMPONENTS graphics net)

add_executable(sailinsane
        sailinsane.cpp src/Protocol.h)

add_executable(server
        src/server/server.cpp src/Protocol.h
        src/server/ServerNetworkHandler.cpp src/server/ServerNetworkHandler.h
        src/server/Player.cpp src/server/Player.h
        src/server/Game.cpp src/server/Game.h
        src/server/BoatWrapper.cpp src/server/BoatWrapper.h
        src/physics/Wind.cpp src/physics/Wind.h
        src/physics/Boat.cpp src/physics/Boat.h
        src/physics/Physics.cpp src/physics/Physics.h)

add_executable(client
        src/client/client.cpp src/Protocol.h
        src/client/ClientNetworkHandler.h src/client/ClientNetworkHandler.cpp
        src/client/BoatEntity.cpp src/client/BoatEntity.h
        src/client/ClientPlayer.cpp src/client/ClientPlayer.h
        )

target_link_libraries(sailinsane
        gf::graphics)

target_link_libraries(server
        gf::net)

target_link_libraries(client
        gf::net
        gf::graphics)
cmake_minimum_required(VERSION 3.0)

set(SAILINSANE_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

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
        src/server/ServerNetworkHandler.cpp
        src/server/ServerNetworkHandler.h
        src/server/Player.cpp
        src/server/Player.h
        src/server/Game.cpp
        src/server/Game.h
        src/server/ServerBoat.cpp
        src/server/ServerBoat.h
        src/physics/Wind.cpp
        src/physics/Wind.h
        src/physics/Boat.cpp
        src/physics/Boat.h
        src/physics/Physics.cpp
        src/physics/Physics.h
        src/physics/BoatControl.cpp
        src/physics/BoatControl.h src/server/World.cpp src/server/World.h src/Constants.h src/server/GameMessages.h src/server/Managers.cpp src/server/Managers.h)

add_executable(client
        src/client/client.cpp src/Protocol.h
        src/client/ClientNetworkHandler.h
        src/client/ClientNetworkHandler.cpp
        src/client/ClientBoat.cpp
        src/client/ClientBoat.h
        src/client/ClientPlayer.cpp
        src/client/ClientPlayer.h
        src/physics/Boat.cpp
        src/physics/Boat.h
        src/physics/BoatControl.cpp
        src/physics/BoatControl.h
        src/client/Singletons.cpp
        src/client/Singletons.h
        src/client/NavigationArrow.cpp
        src/client/NavigationArrow.h src/client/Terrain.cpp src/client/Terrain.h src/Constants.h src/client/WindArrow.cpp src/client/WindArrow.h src/client/Banner.h src/client/Banner.cpp src/LoggingUtils.h)

target_include_directories(client
        PRIVATE
          ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(sailinsane
        gf::graphics)

target_link_libraries(server
        gf::net)

target_link_libraries(client
        gf::net
        gf::graphics)
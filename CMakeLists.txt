cmake_minimum_required(VERSION 3.0)

project(SAILINSANE
  VERSION 0.1.0
)

find_package(gf REQUIRED COMPONENTS graphics net)

add_executable(sailinsane
        sailinsane.cpp Protocol.h)

add_executable(server
        server.cpp Protocol.h)

add_executable(client
        client.cpp Protocol.h)

target_link_libraries(sailinsane
        gf::graphics)

target_link_libraries(server
        gf::net)

target_link_libraries(client
        gf::net)
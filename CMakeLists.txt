cmake_minimum_required(VERSION 3.0)
project(SAILINSANE
  VERSION 0.1.0
)
find_package(gf REQUIRED)
add_executable(sailinsane
        sailinsane.cpp)
target_link_libraries(sailinsane
        gf::graphics
)
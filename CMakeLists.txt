cmake_minimum_required(VERSION 3.10)
project(Game-Networking VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wno-switch -Wno-sign-compare")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/etc
    ${CMAKE_CURRENT_SOURCE_DIR}/etc/googletest-1.16.0/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/etc/googletest-1.16.0/lib
)

# Make the library a shared library since we are going to be linking against many test cases, etc
set(GAME_NETWORKING_SRCS src/Player.cpp)
add_library(game-networking SHARED ${GAME_NETWORKING_SRCS})

add_executable(gameClient src/gameClient.cpp)
add_executable(gameServer src/gameServer.cpp)

# Link the library to the gameServer and gameClient executable
target_link_libraries(gameServer game-networking)
target_link_libraries(gameClient game-networking)

# Testing executables
add_executable(playerTest test/test_Player.cpp)
target_link_libraries(playerTest gtest game-networking)
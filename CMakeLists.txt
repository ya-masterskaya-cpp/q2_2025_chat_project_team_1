cmake_minimum_required(VERSION 3.16)
project(IRC-chat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(domain)
set(DOMAIN_SET true)
add_subdirectory(server)

option(BUILD_CLIENT "Build the GUI client" ON)
if(BUILD_CLIENT)
    add_subdirectory(client)
endif()
cmake_minimum_required(VERSION 3.31)
project(IRC-chat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
set(wxBUILD_SHARED ON)
include(${wxWidgets_USE_FILE})

#настройка boost
find_package(Boost REQUIRED COMPONENTS system filesystem)

#настройка libpqxx
find_package(libpqxx REQUIRED)

#настройка целей
set(CLIENT_INCL client/include)
set(CLIENT_SRC client/src)
set(SERVER_INCL server/include)
set(SERVER_SRC server/src)

add_executable(Client
    ${CLIENT_SRC}/main.cpp
    ${CLIENT_INCL}/application.h
    ${CLIENT_INCL}/main_frame.h
    ${CLIENT_SRC}/main_frame.cpp

)
target_include_directories(Client PRIVATE ${CLIENT_INCL})

add_executable(Server
    ${SERVER_SRC}/main.cpp
)
target_include_directories(Server PRIVATE ${SERVER_INCL})


target_link_libraries(Client PRIVATE ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(Server PRIVATE ${Boost_LIBRARIES} libpqxx::pqxx)

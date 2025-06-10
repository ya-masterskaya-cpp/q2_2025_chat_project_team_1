cmake_minimum_required(VERSION 3.31)
project(IRC-chat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
set(wxBUILD_SHARED ON)
include(${wxWidgets_USE_FILE})

#настройка boost
find_package(Boost REQUIRED COMPONENTS system filesystem json serialization)

#настройка libpqxx
find_package(libpqxx REQUIRED)

#настройка целей
set(CLIENT_INCL client/include)
set(CLIENT_SRC client/src)
set(SERVER_INCL server/include)
set(SERVER_SRC server/src)
set(DOMAIN_INCL domain/include)
set(DOMAIN_SRC domain/src)

#все *.cpp и *.h для сервера 
file(GLOB DOMAIN_SOURCES "domain/src/*.cpp" "domain/include/*.h")
file(GLOB SERVER_SRC_FILES "${SERVER_SRC}/*.cpp" "${SERVER_SRC}/*.h")

add_executable( Client
    ${CLIENT_SRC}/main.cpp
)
target_include_directories(Client PRIVATE ${CLIENT_INCL})
target_include_directories(Client PRIVATE ${DOMAIN_INCL})

#add_executable(Server
#    ${SERVER_SRC}/main.cpp
#)
add_executable(Server
    ${SERVER_SRC_FILES}
)
target_include_directories(Server PRIVATE ${SERVER_INCL})
target_include_directories(Server PRIVATE ${DOMAIN_INCL})


add_library(general_domain_lib STATIC
    ${DOMAIN_SOURCES}
)
target_include_directories(general_domain_lib PRIVATE ${DOMAIN_INCL})

target_link_libraries(Client PRIVATE ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} general_domain_lib)
target_link_libraries(Server PRIVATE ${Boost_LIBRARIES} libpqxx::pqxx general_domain_lib)
target_link_libraries(general_domain_lib PRIVATE ${Boost_LIBRARIES})
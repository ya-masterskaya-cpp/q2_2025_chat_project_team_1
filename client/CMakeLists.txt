cmake_minimum_required(VERSION 3.16)
project(client)

option(BUILD_CLIENT "Build the GUI client" ON)
if(NOT BUILD_CLIENT)
    return()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpr REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(ixwebsocket REQUIRED)
find_package(wxWidgets COMPONENTS core base html xml richtext REQUIRED)
set(wxBUILD_SHARED ON)
include(${wxWidgets_USE_FILE})

if(wxWidgets_FOUND)
    set(CLIENT_INCL ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(CLIENT_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set(DOMAIN_INCL ${CMAKE_CURRENT_SOURCE_DIR}/../domain/include)

    file(GLOB_RECURSE CLIENT_INCL_FILES ${CLIENT_INCL}/*.h)
    file(GLOB_RECURSE CLIENT_SRC_FILES  ${CLIENT_SRC}/*.cpp)




    if(NOT DEFINED DOMAIN_SET)
        get_filename_component(DOMAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../domain" ABSOLUTE)
        add_subdirectory(${DOMAIN_DIR} ${CMAKE_BINARY_DIR}/domain)
    endif()

    add_executable(Client
        ${CLIENT_SRC_FILES}
        ${CLIENT_INCL_FILES}
    )


    target_include_directories(Client PRIVATE ${CLIENT_INCL} ${DOMAIN_INCL})
    target_link_libraries(Client PRIVATE ${wxWidgets_LIBRARIES} cpr::cpr jsoncpp_static ixwebsocket::ixwebsocket general_domain_lib)

    if(UNIX)
        find_package(X11 REQUIRED)
        target_link_libraries(Client PRIVATE ${X11_LIBRARIES})
    endif()
endif()

install(TARGETS Client DESTINATION bin)
link_directories(/app/lib)

# ---- TESTS ----
find_package(GTest REQUIRED)
set(TESTS_INCL ${CMAKE_CURRENT_SOURCE_DIR}/tests/include)
set(TESTS_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/tests/src)

file(GLOB_RECURSE TESTS_INCL_FILES ${TESTS_INCL}/*.h)
file(GLOB_RECURSE TESTS_SRC_FILES  ${TESTS_SRC}/*.cpp)

add_executable(Client_tests
    ${TESTS_SRC_FILES}
    ${TESTS_INCL_FILES}
    ${CLIENT_SRC}/websocket_client.cpp
)

target_include_directories(Client_tests PRIVATE ${TESTS_INCL} ${CLIENT_INCL})
target_link_libraries(Client_tests PRIVATE ${wxWidgets_LIBRARIES} cpr::cpr jsoncpp_static ixwebsocket::ixwebsocket general_domain_lib GTest::gtest GTest::gmock GTest::gtest_main)

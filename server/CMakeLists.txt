cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)

set(SERVER_INCL ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SERVER_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DOMAIN_INCL ${CMAKE_CURRENT_SOURCE_DIR}/../domain/include)

file(GLOB_RECURSE SERVER_INCL_FILES ${SERVER_INCL}/*.h)
file(GLOB_RECURSE SERVER_SRC_FILES  ${SERVER_SRC}/*.cpp)

if(NOT DEFINED DOMAIN_SET)
    get_filename_component(DOMAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../domain" ABSOLUTE)
    add_subdirectory(${DOMAIN_DIR} ${CMAKE_BINARY_DIR}/domain)
endif()

add_executable(Server
    ${SERVER_SRC_FILES}
    ${SERVER_INCL_FILES}
)

target_include_directories(Server PRIVATE ${SERVER_INCL} ${DOMAIN_INCL})

target_link_libraries(Server PRIVATE Drogon::Drogon general_domain_lib libpqxx::pqxx PostgreSQL::PostgreSQL)

# ---- TESTS ----
enable_testing()

# Тест для БД
add_executable(DbTests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_postgres.cpp
    ${SERVER_SRC}/postgres.cpp
    ${SERVER_SRC}/db_wrapper.cpp
    ${SERVER_SRC}/tagged_uuid.cpp
)

find_package(GTest REQUIRED)

target_include_directories(DbTests PRIVATE ${SERVER_INCL})
target_link_libraries(DbTests PRIVATE libpqxx::pqxx PostgreSQL::PostgreSQL)
target_link_libraries(DbTests PRIVATE GTest::gtest GTest::gtest_main pthread)
add_test(NAME DbTestRun COMMAND DbTests)

# Тест для TokenManager
add_executable(TokenManagerTests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_token_manager.cpp
    ${SERVER_SRC}/token_manager.cpp
    ${SERVER_SRC}/tagged_uuid.cpp
 )

 target_include_directories(TokenManagerTests PRIVATE ${SERVER_INCL})
 target_link_libraries(TokenManagerTests PRIVATE GTest::gtest GTest::gtest_main pthread PRIVATE libpqxx::pqxx PostgreSQL::PostgreSQL)
 add_test(NAME TokenManagerTestRun COMMAND TokenManagerTests)
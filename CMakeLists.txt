cmake_minimum_required(VERSION 3.16)
project(ServerApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# !!! Toolchain-файл задаётся только в командной строке или настройках IDE !!!

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(Boost REQUIRED)

# ==== Основное приложение ====
add_executable(Server
    server/src/main.cpp
    server/src/postgres.cpp
    server/src/tagged_uuid.cpp
)

target_include_directories(Server PRIVATE
    ${PQXX_INCLUDE_DIRS}
    ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(Server PRIVATE
    ${PQXX_LIBRARIES}
    ${wxWidgets_LIBRARIES}
    Boost::boost
)

target_compile_options(Server PRIVATE -Wall -Wextra -Wpedantic)

# ==== Юнит-тесты ====
# Найдём GoogleTest (желательно через vcpkg, либо через систему)
find_package(GTest REQUIRED)

add_executable(run_tests
    server/tests/test_postgres.cpp
    server/src/postgres.cpp
    server/src/tagged_uuid.cpp
    # Если появятся другие cpp-файлы ядра, добавляй сюда!
)

target_include_directories(run_tests PRIVATE
    ${PQXX_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    ${wxWidgets_INCLUDE_DIRS}
    server/src
    server/tests
)

target_link_libraries(run_tests PRIVATE
    ${PQXX_LIBRARIES}
    ${wxWidgets_LIBRARIES}
    Boost::boost
    GTest::GTest
    GTest::Main
    pthread
)

target_compile_options(run_tests PRIVATE -Wall -Wextra -Wpedantic)

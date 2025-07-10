cmake_minimum_required(VERSION 3.16)

find_package(OpenSSL REQUIRED)

set(DOMAIN_INCL ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DOMAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE DOMAIN_SOURCES "${DOMAIN_SRC}/*.cpp" "${DOMAIN_INCL}/*.h")

add_library(general_domain_lib STATIC ${DOMAIN_SOURCES})
target_include_directories(general_domain_lib PUBLIC ${DOMAIN_INCL})
target_link_libraries(general_domain_lib PRIVATE OpenSSL::SSL OpenSSL::Crypto)

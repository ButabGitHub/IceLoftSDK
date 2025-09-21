cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)

project(TestIclGame)

add_subdirectory(vendor/IceLoftSDK/IceLoftSDK)

file(GLOB_RECURSE PROJ_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJ_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(TestIclGame ${PROJ_SOURCES} ${PROJ_HEADERS})

target_link_libraries(TestIclGame PRIVATE IceLoftEngine)

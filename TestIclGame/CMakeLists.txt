cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)

project(TestIclGame)

add_subdirectory(vendor/IceLoftSDK/IceLoftSDK)

file(GLOB_RECURSE PROJ_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJ_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES 
		${PROJ_HEADERS} ${PROJ_SOURCES}
)

add_executable(TestIclGame ${PROJ_SOURCES} ${PROJ_HEADERS})

target_sources(TestIclGame PRIVATE ${PROJ_SOURCES} ${PROJ_HEADERS})
target_include_directories(TestIclGame
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

set_target_properties(TestIclGame PROPERTIES FOLDER "Games")

# target_link_libraries(TestIclGame PRIVATE IceLoftEngine)

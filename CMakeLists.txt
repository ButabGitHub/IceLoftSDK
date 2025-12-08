cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)

project(IceLoftEngine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(ThirdParty/glfw)
add_subdirectory(ThirdParty/glad)
add_subdirectory(ThirdParty/glm)
add_subdirectory(ThirdParty/stb_image)

# Specify source and header files
file(GLOB_RECURSE ICL_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE ICL_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES 
		${ICL_HEADERS} ${ICL_SOURCES}
)

add_library(IceLoftEngine STATIC ${ICL_SOURCES} ${ICL_HEADERS})

target_sources(IceLoftEngine PRIVATE ${ICL_SOURCES} ${ICL_HEADERS})
target_include_directories(IceLoftEngine
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

target_link_libraries(IceLoftEngine PUBLIC glad glm stb_image)
set_target_properties(IceLoftEngine PROPERTIES FOLDER "Engine")

set_target_properties(glad glm stb_image PROPERTIES FOLDER "Dependencies")

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    target_link_libraries(IceLoftEngine PUBLIC OpenGL::GL)
endif()
cmake_minimum_required(VERSION 3.10)

project(CSRP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB csrp_SRC
     "src/*.h"
     "src/*.cpp"
)

add_executable(csrp ${csrp_SRC})

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/extern/cxxopts/include)

set(xmlrpc-c_SOURCE_DIR ${PROJECT_SOURCE_DIR}/extern/xmlrpc-c)
add_subdirectory(${xmlrpc-c_SOURCE_DIR})
include_directories(${xmlrpc-c_SOURCE_DIR}/include)

target_link_libraries(csrp xmlrpc_client++)

if (WIN32)
    cmake_policy(SET CMP0079 NEW) # allow adding target_link_libraries to imported targets
    target_link_libraries(xmlrpc_client ws2_32 curl)
    target_link_libraries(xmlrpc_util ws2_32)

    target_link_libraries(csrp ws2_32)
endif()

cmake_minimum_required(VERSION 3.10)
project(ConcurrentWebServer)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") 

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/ConcurrentWebServer)
include_directories(${PROJECT_SOURCE_DIR}/WebServerTests)

# Check if Google Test is already cloned
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/googletest")
    message(STATUS "Google Test not found. Cloning...")
    include(ExternalProject)
    ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
        PREFIX ${CMAKE_BINARY_DIR}/external
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external
    )
    set(GTEST_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/external/include)
    set(GTEST_LIBS_DIR ${CMAKE_BINARY_DIR}/external/lib)
    include_directories(${GTEST_INCLUDE_DIRS})
else()
    message(STATUS "Google Test found.")
    add_subdirectory(external/googletest)
    include_directories(${PROJECT_SOURCE_DIR}/external/googletest/googletest/include)
endif()

# Add source files
set(SERVER_CLIENT_SOURCES 
ConcurrentWebServer/Server.cpp
ConcurrentWebServer/Client.cpp
ConcurrentWebServer/RequestHandler.cpp
ConcurrentWebServer/SocketUtils.cpp
ConcurrentWebServer/ThreadPool.cpp
ConcurrentWebServer/main.cpp
)

# Create executable from server sources
add_executable(ConcurrentWebServer ${SERVER_CLIENT_SOURCES})

# Create static library from server sources
add_library(ConcurrentWebServer_lib STATIC ${SERVER_CLIENT_SOURCES})

# Link Winsock library
target_link_libraries(ConcurrentWebServer_lib ws2_32)

# Add test sources
set(TEST_SOURCES
WebServerTests/test.cpp
)

# Create test executable
add_executable(runTests ${TEST_SOURCES})

# Link the test executable against Google Test and server library
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/googletest")
    add_dependencies(runTests googletest)
    target_link_libraries(runTests ${GTEST_LIBS_DIR}/gtest.lib ${GTEST_LIBS_DIR}/gtest_main.lib ConcurrentWebServer_lib ws2_32)
else()
    target_link_libraries(runTests gtest gtest_main ConcurrentWebServer_lib ws2_32)
endif()

# Enable testing
enable_testing()

# Add test
add_test(NAME runTests COMMAND runTests)
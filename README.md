# Concurrent Web Server

This project implements a concurrent web server using C++14. The server is designed to handle multiple client connections simultaneously by using a thread pool.

## Features

- Concurrent handling of client connections.
- Thread pool for efficient management of threads.
- Basic HTTP request handling.

## Requirements

- CMake 3.10 or higher
- Visual Studio 2019 or 2022 (or another C++14 compatible compiler)
- Google Test (for running tests)

## Building the Project

### Cloning the Repository

First, clone the repository along with the Google Test submodule:

```sh
git clone --recurse-submodules https://github.com/irapilguy/ConcurrentWebServer.git
```

### Generating Build Files

Navigate to the project directory and create a build directory:

```sh
cd ConcurrentWebServer
mkdir build
cd build
```
Generate the build files using CMake:

For Visual Studio 2019:

```sh
cmake -G "Visual Studio 16 2019" ..
```

For Visual Studio 2022:

```sh
cmake -G "Visual Studio 17 2022" ..
```
### Building the Project

Build the project using CMake:

```sh
cmake --build . --config Release
```

### Running the App

Navigate to the build directory and run the executable:

```sh
cd Release
ConcurrentWebServer.exe
```
## Running Tests

To run the tests, first, ensure that the project is built in the debug configuration:
```sh
cmake --build . --config Debug
```

Run the tests using CTest:
```sh
ctest --output-on-failure
```
## Directory Structure
```arduino
ConcurrentWebServer/
├── ConcurrentWebServer/
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── RequestHandler.cpp
│   ├── SocketUtils.cpp
│   ├── ThreadPool.cpp
│   ├── Server.h
│   ├── Client.h
│   ├── RequestHandler.h
│   ├── SocketUtils.h
│   ├── ThreadPool.h
│   └── main.cpp
├── WebServerTests/
│   └── test.cpp
├── external/
│   └── googletest/
│       └── googletest/
│           └── include/
├── build/
├── CMakeLists.txt
└── README.md
```

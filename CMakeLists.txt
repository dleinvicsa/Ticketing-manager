cmake_minimum_required(VERSION 3.10)
project(GroupProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(lib)

# Source files
set(SOURCES
    src/main.cpp
    src/auth/AuthService.cpp
    src/core/BookingManager.cpp
    src/core/Database.cpp
    src/transport/RS_transport.cpp
    src/transport/Vivi.cpp
)

# Create executable
add_executable(GroupProject ${SOURCES})

# Link libraries (Threads is usually required for Crow/Asio)
find_package(Threads REQUIRED)
target_link_libraries(GroupProject PRIVATE Threads::Threads)

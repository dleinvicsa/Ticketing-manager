# Ticketing Manager

A C++/Qt-based transport ticketing management system for booking train and bus trips.

## Features

- **User Authentication**: Secure Login & Registration system.
- **Ticket Booking**: Search and book trips for various transport types.
- **Balance Management**: "Top Up" feature for account balance.
- **Admin Panel**: User management interface for administrative tasks.
- **Data Persistence**: JSON-based storage for users and tickets.

## Prerequisites

- **Qt 6.x** (Widgets module)
- **CMake 3.16+**
- **C++17 Compiler** (Clang/GCC for macOS, MSVC for Windows)
- **nlohmann/json** (Included in `include/nlohmann/`)

## Build Instructions

### macOS
1. Open terminal in project root.
2. Create build directory:
   ```bash
   mkdir build && cd build
   ```
3. Generate and build:
   ```bash
   cmake ..
   make
   ```
4. Run:
   ```bash
   ./transport_app
   ```

### Windows
1. Open **Developer Command Prompt for VS** or **PowerShell**.
2. Create build directory:
   ```powershell
   mkdir build; cd build
   ```
3. Generate and build:
   ```powershell
   cmake ..
   cmake --build . --config Release
   ```
4. Run:
   ```powershell
   .\Release\transport_app.exe
   ```

## Project Structure

- `include/`: Header files organized by module (`auth`, `core`, `transport`, `ui`).
- `src/`: Implementation files for authentication, core logic, and UI.
- `js/data/`: JSON database files (`users.json`, `tickets.json`).
- `CMakeLists.txt`: Build configuration file.

## Development
For VS Code users, it is recommended to use the **CMake Tools** extension for automated building and debugging.

cmake_minimum_required(VERSION 3.16)
project(TicketingManager)
set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Threads REQUIRED)
set(CMAKE_AUTOMOC ON)
qt_add_executable(transport_app
    src/main.cpp
    src/ui/mainwindow.cpp
    include/ui/mainwindow.h
    src/auth/AuthService.cpp
    src/auth/User.cpp
    src/core/BookingManager.cpp
    src/core/Database.cpp
    include/ui/bookingdialog.h
    src/ui/bookingdialog.cpp
    include/ui/logindialog.h
    src/ui/logindialog.cpp
    include/ui/registerdialog.h
    src/ui/registerdialog.cpp
    include/ui/ticketdialog.h
    src/ui/ticketdialog.cpp
)
file(COPY js DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(transport_app PRIVATE
    include
    include/auth
    include/core
    include/transport
    include/ui
    include/nlohmann
)
target_link_libraries(transport_app PRIVATE
    Qt6::Widgets
    Threads::Threads
)
cmake_minimum_required(VERSION 3.14)
project(MonProjet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR "C:/Qt/6.9.0/mingw_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

set(SOURCES
    main.cpp
    ui/speed.cpp
    ui/speed.h
    ui/temperaturecontrol.cpp
    ui/temperaturecontrol.h
    ui/timercontrol.cpp
    ui/timercontrol.h
    
)


add_executable(MonProjet ${SOURCES})

target_link_libraries(MonProjet PRIVATE Qt6::Widgets  Qt6::Multimedia)



set_target_properties(MonProjet PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

cmake_minimum_required(VERSION 3.16)
project(NothingHere VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

if(WIN32)
    set(PLATFORM_SOURCES 
        src/platform/windows/keyboardhook.cpp
        src/platform/windows/hidewindows.cpp
        src/platform/windows/systemvolume.cpp
    )
    set(PLATFORM_HEADERS 
        src/platform/windows/keyboardhook.h
        src/platform/windows/hidewindows.h
        src/platform/windows/systemvolume.h
    )
endif()

add_executable(${PROJECT_NAME} WIN32
    main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/panicbutton.cpp
    src/panicbutton.h
    src/settings.cpp
    src/settings.h
    src/trayicon.cpp
    src/trayicon.h
    src/globalhotkey.cpp
    src/globalhotkey.h
    ${PLATFORM_SOURCES}
    ${PLATFORM_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE user32.lib)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DISABLE_DEPRECATED_BEFORE=0x050F00
    $<$<CONFIG:Release>:QT_NO_DEBUG_OUTPUT>
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

cmake_minimum_required(VERSION 3.5)

project(Qt_GUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}Multimedia)  # <-------- This Multimedia

set(PROJECT_SOURCES
        main.cpp
        src/cpu/cpu.cpp
        src/cpu/cpu.h
        src/emulator/emulator.cpp
        src/emulator/emulator.h
        src/gui/gui.cpp
        src/gui/gui.h
        src/flags/flags.cpp
        src/flags/flags.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Qt_GUI
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Qt_GUI SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Qt_GUI res.qrc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Qt_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt5::Multimedia)

cmake_minimum_required(VERSION 3.16)

project(libtle LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(libtle STATIC
  libtle.cpp
  libtle.h
)

target_link_libraries(libtle PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(libtle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(libtle PRIVATE LIBTLE_LIBRARY)

# Добавляем зависимость между двумя библиотеками:
target_link_libraries(libtle PUBLIC libbase)

cmake_minimum_required(VERSION 3.16)

project(ObjectListProvider LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ObjectListProvider STATIC
  objectlistprovider.cpp
  objectlistprovider.h
)

target_compile_definitions(ObjectListProvider PRIVATE OBJECTLISTPROVIDER_LIBRARY)

target_link_libraries(ObjectListProvider
    PUBLIC Qt6::Core Qt6::Positioning
)

target_include_directories(ObjectListProvider PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

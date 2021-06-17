cmake_minimum_required(VERSION 3.10)
project(CameraDistortion)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std:c++17")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -Wall -Wextra -DDEBUG")
endif()

find_package(OpenCV 4 REQUIRED)

include_directories(include)

add_executable(distort
        include/CameraDistortion.hpp
        src/mainDistortImage.cpp
        src/CameraDistortion.cpp)

target_link_libraries(distort PRIVATE ${OpenCV_LIBS})

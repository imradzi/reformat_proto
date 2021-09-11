cmake_minimum_required(VERSION 3.21)
project(reformat_proto VERSION 0.9.0)

set(CMAKE_CXX_STANDARD 20)
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /DwxUSE_STL /MP /bigobj /D_UNICODE /D__WX__ /D__WXMSW__ /DWIN32 /DNOMINMAX")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} Threads::Threads)

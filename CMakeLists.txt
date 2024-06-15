cmake_minimum_required(VERSION 3.10)

project(Tello_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure position-independent code is enabled globally
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.8.5  # Replace with the desired version
)

FetchContent_MakeAvailable(spdlog)

find_package(OpenCV REQUIRED)

# CTello Shared Library =======================================================
add_library(tello SHARED tello.cpp)

target_include_directories(tello PRIVATE include)
target_link_libraries(tello PRIVATE spdlog::spdlog ${OpenCV_LIBS})

install(TARGETS tello DESTINATION lib)
install(FILES include/tello.h DESTINATION include)

# CTello State ================================================================

add_executable(tello_state tello_state.cpp)

target_include_directories(tello_state PRIVATE include)
target_link_libraries(tello_state PRIVATE tello ${OpenCV_LIBS})

install(TARGETS tello_state DESTINATION bin)

# Tello Stream ===============================================================

add_executable(tello_stream tello_stream.cpp)

target_include_directories(tello_stream PRIVATE include)
target_link_libraries(tello_stream PRIVATE tello ${OpenCV_LIBS})

install(TARGETS tello_stream DESTINATION bin)







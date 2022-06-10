cmake_minimum_required(VERSION 3.20)
project(rushhour)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:\\GCC_Compiler\\SFML-2.5.1x32\\lib\\cmake\\SFML")

# Include the SFML library and link it static
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)


include_directories(${PROJECT_SOURCE_DIR}/sourcefiles)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/sourcefiles/*.cpp)

add_executable(rushhour ${SRC_FILES})
target_link_libraries(rushhour sfml-graphics sfml-audio)
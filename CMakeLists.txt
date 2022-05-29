cmake_minimum_required(VERSION 3.20)
project(rushhour)



set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:\\GCC_Compiler\\SFML-2.5.1x32\\lib\\cmake\\SFML")

# Include the SFML library and link it static
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(rushhour main.cpp Car.cpp Car.hpp LevelHandler.cpp LevelHandler.h GameSettings.hpp)

target_link_libraries(rushhour sfml-graphics)

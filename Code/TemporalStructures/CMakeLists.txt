cmake_minimum_required(VERSION 3.10)
project(TemporalStructures LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Libreria statica
add_library(temporalStructures STATIC
    lib/temporalStructures.cpp
)

target_include_directories(temporalStructures PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Bisogna aggiungere qui i nuovi file che devono essere compilati
add_executable(main
    src/main.cpp
)
target_link_libraries(main PRIVATE temporalStructures)

add_executable(windowAlg
    src/windowAlg.cpp
)
target_link_libraries(windowAlg PRIVATE temporalStructures)



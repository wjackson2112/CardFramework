cmake_minimum_required(VERSION 3.18)

project(CardFramework)

add_library(CardFramework)

target_link_libraries(CardFramework
                      PUBLIC
                      GameEngine)

add_subdirectory(assets)
add_subdirectory(source)
add_subdirectory(shaders)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++)
endif()


cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -pedantic -g ")


project(pi-launcher VERSION 1.0)

## MY FILES
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(pi-launcher ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${SRC_FILES})
target_include_directories(pi-launcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create an option to switch between a system sdl library and a vendored sdl library
option(SDL_VENDORED "Use vendored libraries" ON)

if(SDL_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
    add_subdirectory(vendored/sdl_image EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

    find_package(SDL2_image REQUIRED)
endif()

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(pi-launcher PRIVATE SDL2::SDL2main)
endif()


# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(pi-launcher PRIVATE SDL2::SDL2)

target_link_libraries(pi-launcher PRIVATE SDL2_image::SDL2_image)

if (WIN32)
    add_custom_command(
        TARGET pi-launcher POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:pi-launcher>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2_image::SDL2_image>" "$<TARGET_FILE_DIR:pi-launcher>"
        VERBATIM
    )
endif()
cmake_minimum_required(VERSION 3.5)
project(demo)
set(CMAKE_CXX_STANDARD 11)

# Set paths
set(PATH_LIB /home/thuhuong/Documents/final-demo/generated)
set(PATH_CUSTOM /home/thuhuong/Documents/final-demo/custom)

# Set compiler
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)

# Suppress warnings for GCC
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.1)
    add_compile_options(-Wno-psabi)
endif()

# Adjust the default behavior of the find commands
# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# Search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PATH_CUSTOM}
    ${PATH_LIB}
    ${PATH_LIB}/lvgl/
    ${PATH_LIB}/lv_drivers/
)

add_definitions(-DUSE_LOG_FILE)

message(STATUS "HOME:=$ENV{HOME}")
message(STATUS "CMAKE_BUILD_TYPE:=${CMAKE_BUILD_TYPE}")
message(STATUS "PLATFORM:=${PLATFORM}")

# Set sources
file(GLOB_RECURSE IMAGE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/images/*.c)
file(GLOB_RECURSE FONT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/guider_fonts/*.c)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/setup_scr_screen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gui_guider.c
    ${CMAKE_CURRENT_SOURCE_DIR}/events_init.c
)

# Library sources
file(GLOB_RECURSE LIB_SOURCES ${PATH_LIB}/lvgl/*.c)
file(GLOB_RECURSE LIB_SOURCES_DRIVER ${PATH_LIB}/lv_drivers/*.c)

# Build libraries
add_library(lvgl STATIC ${LIB_SOURCES})
add_library(lv_drivers STATIC ${LIB_SOURCES_DRIVER})

# Build executable
add_executable(demo main.cc ${SOURCES} ${IMAGE_SOURCES} ${FONT_SOURCES})

# Link libraries
target_link_libraries(demo pthread lvgl lv_drivers mosquitto)

# Install target (if needed)
# install(TARGETS demo RUNTIME DESTINATION bin)

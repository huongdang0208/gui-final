cmake_minimum_required(VERSION 3.5)
project(hub-screen)


message("Cmake from Bui Dinh Hien")
# set path
set(PATH_LIB /home/livinglab/work/gui-final/generated)
set(PATH_CUSTOM /home/livinglab/work/gui-final/custom)


set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)
# Set compiler
# set(CMAKE_CXX_COMPILER /home/bhien/work/bbb/gcc/bin/arm-linux-gnueabihf-g++)
# set(CMAKE_C_COMPILER /home/bhien/work/bbb/gcc/bin/arm-linux-gnueabihf-gcc)

# Suppress warnings for GCC
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.1)
    add_compile_options(-Wno-psabi)
endif()

# Adjust the default behaviour of the find commands:
# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# Search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)


# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/transport/mqtt
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/hub
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
    ${CMAKE_CURRENT_SOURCE_DIR}/transport/mqtt/mqtt.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/hub/typedef.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gui_guider.c
    ${CMAKE_CURRENT_SOURCE_DIR}/events_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hub_screen.cc
)

# Library sources
file(GLOB_RECURSE LIB_SOURCES ${PATH_LIB}/lvgl/*.c)
file(GLOB_RECURSE LIB_SOURCES_DRIVER ${PATH_LIB}/lv_drivers/*.c)

# Build libraries
add_library(lvgl STATIC ${LIB_SOURCES})
add_library(lv_drivers STATIC ${LIB_SOURCES_DRIVER})
#add_library(lumi STATIC ${SOURCES_LUMI})


# Build executable
add_executable(hub-screen main.cc ${SOURCES} ${IMAGE_SOURCES} ${FONT_SOURCES})

# Link libraries
target_link_libraries(hub-screen pthread lvgl lv_drivers mosquitto protobuf pthread)
#install(TARGETS hub-screen RUNTIME DESTINATION bin)

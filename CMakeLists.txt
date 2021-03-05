cmake_minimum_required(VERSION 3.17)
project(MC_EK)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH  pnl-win64-1.10.4)
# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()
# Adding an executable.
add_executable(MC_EK main.cpp)
target_link_libraries(MC_EK ${LIBS})

add_custom_command(TARGET MC_EK POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:MC_EK>)

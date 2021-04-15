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

add_executable(POLY test-polynomial.cpp alglib-3.17.0.cpp.gpl/cpp/src/specialfunctions.cpp alglib-3.17.0.cpp.gpl/cpp/src/ap.cpp alglib-3.17.0.cpp.gpl/cpp/src/alglibinternal.cpp)
file(GLOB ML "alglib-3.17.0.cpp.gpl/cpp/src/*.cpp")

add_executable(REGRESSION test-regression.cpp ${ML})
target_link_libraries(MC_EK ${LIBS})

add_custom_command(TARGET MC_EK POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:MC_EK>)

target_link_libraries(POLY ${LIBS})

add_custom_command(TARGET POLY POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:POLY>)

target_link_libraries(REGRESSION ${LIBS})

add_custom_command(TARGET REGRESSION POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:REGRESSION>)
add_executable(TEST test.cpp ${ML})

target_link_libraries(TEST ${LIBS})

add_custom_command(TARGET TEST POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST>)

add_executable(TEST-MC test/test_path_generation.cpp src/BlackScholesModel.cpp src/BlackScholesModel.h)

target_link_libraries(TEST-MC ${LIBS})

add_custom_command(TARGET TEST-MC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST-MC>)

add_executable(TEST-POLYREG test/test_polyreg.cpp ${ML} src/PolyReg.cpp src/PolyReg.h)

target_link_libraries(TEST-POLYREG ${LIBS})

add_custom_command(TARGET TEST-POLYREG POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST-POLYREG>)

add_executable(TEST-AMC src/AmericanMC.cpp src/BlackScholesModel.cpp src/BlackScholesModel.h src/PolyReg.cpp src/PolyReg.h ${ML})

target_link_libraries(TEST-AMC ${LIBS})

add_custom_command(TARGET TEST-AMC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST-AMC>)

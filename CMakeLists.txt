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




add_executable(TEST-AMC-factorized test/test-AmericanMC.cpp src/BlackScholesModel.cpp src/BlackScholesModel.h src/PolyReg.cpp src/PolyReg.h src/Option.cpp src/Option.h src/VanillaPutOption.cpp src/VanillaPutOption.hpp src/AmericanMonteCarlo.cpp src/AmericanMonteCarlo.hpp  src/GeometricPutOption.cpp src/GeometricPutOption.hpp   utilities/Utilities.cpp ${ML})

target_link_libraries(TEST-AMC-factorized ${LIBS})

add_custom_command(TARGET TEST-AMC-factorized POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST-AMC-factorized>)

add_executable(TEST-AMC-2D test/test-AmericanMC-2d.cpp src/BlackScholesModel.cpp src/BlackScholesModel.h src/PolyReg.cpp src/PolyReg.h src/Option.cpp src/Option.h src/VanillaPutOption.cpp src/VanillaPutOption.hpp src/AmericanMonteCarlo.cpp src/AmericanMonteCarlo.hpp  src/GeometricPutOption.cpp src/GeometricPutOption.hpp   utilities/Utilities.cpp ${ML})

target_link_libraries(TEST-AMC-2D ${LIBS})

add_custom_command(TARGET TEST-AMC-2D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST-AMC-2D>)

add_executable(TEST-GSE test/test_BS_vs_MC_vanilla_geometric_put.cpp src/EuropeanMonteCarlo.cpp src/EuropeanMonteCarlo.hpp src/BlackScholesModel.h src/BlackScholesModel.cpp src/Option.h src/Option.cpp src/VanillaPutOption.hpp src/VanillaPutOption.cpp src/GeometricPutOption.hpp src/GeometricPutOption.cpp ${ML})

target_link_libraries(TEST-GSE ${LIBS})

add_custom_command(TARGET TEST-GSE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:TEST-GSE>)

add_executable(GLOBAL-TEST-2D test/test_BS_vs_MC_vs_AMC_geometric_put.cpp src/AmericanMonteCarlo.hpp src/AmericanMonteCarlo.cpp src/EuropeanMonteCarlo.cpp src/EuropeanMonteCarlo.hpp src/BlackScholesModel.h src/BlackScholesModel.cpp src/Option.h src/Option.cpp src/VanillaPutOption.hpp src/VanillaPutOption.cpp src/GeometricPutOption.hpp src/GeometricPutOption.cpp src/PolyReg.h src/PolyReg.cpp ${ML})

target_link_libraries(GLOBAL-TEST-2D ${LIBS})

add_custom_command(TARGET GLOBAL-TEST-2D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/pnl-win64-1.10.4/lib/"
        $<TARGET_FILE_DIR:GLOBAL-TEST-2D>)

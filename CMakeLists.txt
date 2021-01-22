cmake_minimum_required(VERSION 3.5.1)
project(example)
set(PYBIND11_PYTHON_VERSION 3.6 CACHE STRING "")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
find_package( Cellworld REQUIRED )
find_package( OpenCV 4.0.1 REQUIRED )

set(CLIB_EXEC output)
set(IMAGE_STITCH_EXEC image_stitching)
SET(DRIVER_FLAGS "-DC_GNU64=400 -DOS_LINUX -no-pie")
SET(CMAKE_CXX_FLAGS  "${DRIVER_FLAGS}")
SET(CLIB_SOURCES src/simple_capture.cpp include/controller.h include/detection.h include/image_processing.h)
add_subdirectory(third_party/pybind11)
include_directories(/usr/local/xclib/inc/)


# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_library(${CLIB_EXEC} ${CLIB_SOURCES})

add_executable(${IMAGE_STITCH_EXEC} src/image_stitching.cpp ./src/utility_functions.cpp ./src/image_stitching_main.cpp)
add_executable(sample_capture ./src/record_samples.cpp ./src/record_samples_main.cpp ./src/utility_functions.cpp 
	      ./src/image_stitching.cpp)
add_executable(calibrate_homography ./src/calibrate_camera_homographies.cpp ./src/utility_functions.cpp)
add_executable(back_ground_subt ./src/backGroundSubtraction.cpp ./src/record_samples.cpp ./src/simple_capture.cpp
	       ./src/utility_functions.cpp ./src/image_stitching.cpp ./src/main.cpp src/controller.cpp src/detection.cpp src/image_processing.cpp)

target_include_directories(${IMAGE_STITCH_EXEC} PRIVATE ./include/)
target_include_directories(${CLIB_EXEC} PRIVATE ./include/)
target_include_directories(sample_capture PRIVATE ./include/)
target_include_directories(back_ground_subt PRIVATE ./include/)
target_include_directories(calibrate_homography PRIVATE ./include/)

target_link_libraries(${CLIB_EXEC} ${OpenCV_LIBS})

target_link_libraries(${CLIB_EXEC} /usr/local/xclib/lib/pxipl_x86_64.a  /usr/local/xclib/lib/xclib_x86_64.a m -pthread )
target_link_libraries(back_ground_subt ${OpenCV_LIBS} /usr/local/xclib/lib/pxipl_x86_64.a  /usr/local/xclib/lib/xclib_x86_64.a m -pthread )
target_link_libraries(sample_capture ${OpenCV_LIBS} ${CLIB_EXEC} )
target_link_libraries(${IMAGE_STITCH_EXEC} ${OpenCV_LIBS} ${CLIB_EXEC} )
target_link_libraries(calibrate_homography ${OpenCV_LIBS} ${CLIB_EXEC}  )
target_link_libraries(back_ground_subt cellworld)
target_link_libraries(calibrate_homography cellworld)
target_link_libraries(sample_capture cellworld)
target_link_libraries(${IMAGE_STITCH_EXEC} cellworld)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


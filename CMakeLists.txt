cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 17)

project(maze_cv
		VERSION 2019.1.0
		DESCRIPTION "Agent cell world simulation framework"
		LANGUAGES CXX)

find_package( Cellworld REQUIRED )
find_package( OpenCV 4.0.1 REQUIRED )

###
### MAIN LIBRARY SETUP
###

set( maze_cv_files
	 src/camera.cpp
	 src/controller.cpp
	 src/detection.cpp
	 src/cell_association.cpp include/core.h include/tracking.h src/tracking.cpp include/agent_profile.h src/agent_profile.cpp tools/capture.cpp include/util.h src/util.cpp)


SET(CMAKE_CXX_FLAGS  "-DC_GNU64=400 -DOS_LINUX -no-pie")

add_library(maze_cv ${maze_cv_files})

target_link_libraries(maze_cv ${OpenCV_LIBS})
target_link_libraries(maze_cv /usr/local/xclib/lib/pxipl_x86_64.a  /usr/local/xclib/lib/xclib_x86_64.a m -pthread )
target_link_libraries(maze_cv cellworld)

target_include_directories(maze_cv
		PUBLIC
		include )

target_include_directories(maze_cv
		PUBLIC
		/usr/local/xclib/inc/ )


add_executable(agent_tracking src/agent_tracking.cpp)
target_link_libraries(agent_tracking maze_cv)


add_executable(capture tools/capture.cpp)
target_link_libraries(capture maze_cv)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

###
### TESTS
###
find_package(CatchTests CONFIG QUIET)

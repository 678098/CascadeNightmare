cmake_minimum_required (VERSION 2.6)

project (CascadeNightmare)

add_definitions(-std=gnu++11)

file (GLOB CNIGHTMARE_CORE ${PROJECT_SOURCE_DIR}/*.cpp)


include_directories(${PROJECT_SOURCE_DIR}
)


set (SOURCE "${CNIGHTMARE_CORE};")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g3")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g3")


add_executable(CascadeNightmare ${SOURCE})
target_link_libraries(CascadeNightmare opencv_core opencv_highgui opencv_objdetect opencv_imgcodecs opencv_imgproc)
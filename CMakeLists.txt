# Minimum required
cmake_minimum_required(VERSION 2.8)

# Project name: Posidonia Oceanica Detector (POD)
project(POD)

# Only OpenCV 3 is required
find_package(OpenCV REQUIRED)

# Place binaries in bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# c++11 is required for threaded execution. If compiler does
# not support it, undefine _POD_THREADED_ from include/podefines.h
# end remove the next lines until endif()
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# header files
include_directories(include)

# Executables: trainer (train classifier), detector (classify)
add_executable(trainer src/train_main.cpp src/dirreader.cpp src/helper.cpp src/podescriber.cpp src/potrainer.cpp src/pogroundtruthextractor.cpp)
add_executable(detector src/detect_main.cpp src/dirreader.cpp src/helper.cpp src/podescriber.cpp src/podetector.cpp src/porefiner.cpp)

target_link_libraries(trainer ${OpenCV_LIBS} -lm -lpthread)
target_link_libraries(detector ${OpenCV_LIBS} -lm -lpthread)

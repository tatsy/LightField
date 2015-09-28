cmake_minimum_required(VERSION 3.0.0)
project(LightField)

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Find Qt5
if (WIN32)
  set(Your_Qt5_DIR "Your_Qt5_DIR" CACHE STRING "Your_Qt5_DIR")
  if (NOT EXISTS "${Your_Qt5_DIR}/include/QtCore/qobject.h")
    message(FATAL_ERROR "ERROR: Qt5 not found!!")
  else()
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${Your_Qt5_DIR})
  endif()
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

# Find OpenCV
if (WIN32)
  set(Your_OpenCV_DIR "Your_OpenCV_DIR" CACHE STRING "Your_OpenCV_DIR")
  if (NOT EXISTS "${Your_OpenCV_DIR}/include/opencv2/opencv.hpp")
    message(FATAL_ERROR "ERROR: OpenCV not found!!")
  endif()
endif()

find_package(OpenCV REQUIRED)
  include_directories(${OPENCV_INCLUDE_DIR})

if (OPENCV_FOUND)
endif()

add_subdirectory(LightFieldViewer)
add_subdirectory(LightFieldTexture)
add_subdirectory(LightFieldCamera)

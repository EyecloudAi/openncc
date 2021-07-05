# Copyright (C) 2018-2019 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

file (GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file (GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
link_directories(${LIB_FOLDER} ${CMAKE_CURRENT_SOURCE_DIR})

ie_add_sample(NAME human_pose_estimation_demo
              SOURCES ${SOURCES}
              HEADERS ${HEADERS}
              INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
              DEPENDENCIES monitors
              OPENCV_DEPENDENCIES highgui)

target_link_libraries(human_pose_estimation_demo PRIVATE  OpenNCC usb-1.0)



# CMakeLists.txt
cmake_minimum_required(VERSION 3.12)
project(gTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(SOURCES_DIR "/home/yuance/Work/Encryption/PIR/code/PIR/MPPIR/zerotest/cpp")
# 添加你的项目源文件
add_executable(
    gTest 
    google_test.cpp
    ${SOURCES_DIR}/utils/utils.cpp
    ${SOURCES_DIR}/zerotest.cpp
    ${SOURCES_DIR}/field/mat.cpp
    )

# 链接Google Test和你的项目
target_link_libraries(gTest PRIVATE gtest pthread)


target_include_directories(
    gTest 
    PUBLIC 
    /home/yuance/Work/Encryption/PIR/code/PIR/MPPIR/zerotest/cpp
    )

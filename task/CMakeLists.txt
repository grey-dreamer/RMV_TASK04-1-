cmake_minimum_required(VERSION 3.10) # 设置所需的 CMake 最低版本

# 设置项目名称
project(task)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 OpenCV 库
find_package(OpenCV REQUIRED)
# 查找 Tesseract 库
set(TESSERACT_INCLUDE_DIRS /usr/include/tesseract /usr/include/leptonica)
set(TESSERACT_LIBRARIES tesseract lept)
# Eigen
find_path(EIGEN_INCLUDE_DIR NAMES Eigen/Dense PATHS /usr/local/include/eigen3 /usr/include/eigen3)

# 包含目录
include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}  # 添加 Tesseract 的包含目录
    ${EIGEN_INCLUDE_DIR}
)

# 添加可执行文件
aux_source_directory(./src ALL_SRCS)
add_executable(task main.cpp ${ALL_SRCS}) # 假设你的主程序是 main.cpp

# 链接 OpenCV 库到你的目标
target_link_libraries(task ${OpenCV_LIBS} ${TESSERACT_LIBRARIES})


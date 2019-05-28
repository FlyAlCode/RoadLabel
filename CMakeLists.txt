# 声明CMake的最低要求版本
cmake_minimum_required(VERSION 2.8)

# 定义项目(工程)名称,同时定义了以下默认变量
# PROJECT_SOURCE_DIR 或 HELLO_SOURCE_DIR, 代指CMake开始构建的根目录(通常是项目根目录)
# PROJECT_BINARY_DIR 或 HELLO_BINARY_DIR, 代指CMake的编译目录(即执行cmake命令的目录)
PROJECT(label_road)

find_package(OpenCV 3.3.1 EXACT REQUIRED
            PATHS /opt/ros/kinetic/share/OpenCV-3.3.1-dev)

# 添加lib的查找目录
SET(REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} ${OpenCV_LIBS})
include_directories(${OpenCV_INCLUDE_DIRS} )

include_directories(${PROJECT_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(src)

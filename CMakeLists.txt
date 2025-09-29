cmake_minimum_required(VERSION 3.10.0)
project(Task03 VERSION 0.1.0 LANGUAGES C CXX)

# 查找源文件（建议大型项目显式列出文件而非使用GLOB）
file(GLOB_RECURSE SRC_List "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# 配置输出路径和C++标准
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 确保标准被严格遵守
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 显式指定库的CMake配置路径（如果需要）
set(OpenCV_DIR "/usr/local/lib/cmake/opencv4")
set(Ceres_DIR "/usr/local/lib/cmake/Ceres")

# 查找依赖库
find_package(OpenCV REQUIRED)
find_package(Ceres REQUIRED)  # Ceres会自动查找Eigen依赖

# 创建可执行目标
add_executable(app ${SRC_List})

# 设置包含目录
target_include_directories(app PRIVATE
    ${OpenCV_INCLUDE_DIRS}  # 注意变量名是OpenCV_INCLUDE_DIRS（复数形式）
    ${CERES_INCLUDE_DIRS}   # Ceres的包含目录变量是CERES_INCLUDE_DIRS
    # Eigen的包含目录不需要单独列出，Ceres会自动传递
)

# 编译选项
target_compile_options(app PRIVATE -Wall -Wextra)

# 链接依赖库
target_link_libraries(app 
    ${OpenCV_LIBS}       # OpenCV推荐使用这个变量
    ${CERES_LIBRARIES}        # 使用现代CMake的目标链接方式
)

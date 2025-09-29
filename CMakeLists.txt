cmake_minimum_required(VERSION 3.10.0)
project(opencv_project VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SRC_List "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(OpenCV_DIR "/usr/local/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

# 新增：查找 Tesseract 和 Leptonica 库
find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract)
pkg_check_modules(LEPTONICA REQUIRED lept)

# 调试信息，确认库路径
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
message(STATUS "Tesseract_INCLUDE_DIRS = ${TESSERACT_INCLUDE_DIRS}")
message(STATUS "Tesseract_LIBRARIES = ${TESSERACT_LIBRARIES}")

add_executable(app ${SRC_List})

# 链接所有依赖库
target_include_directories(app PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
    ${LEPTONICA_INCLUDE_DIRS}
)

target_compile_options(app PRIVATE -Wall -Wextra)
target_link_libraries(app 
    ${OpenCV_LIBS}
    ${TESSERACT_LIBRARIES}
    ${LEPTONICA_LIBRARIES}
)

cmake_minimum_required(VERSION 3.2)
project(iscore_plugin_coppa)

add_subdirectory(coppa)
add_subdirectory(i-score)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
cmake_policy(SET CMP0042 NEW)

# Packages
find_package(Qt5 5.4 REQUIRED COMPONENTS Core Widgets)

# For coppa
include_directories("${CMAKE_SOURCE_DIR}/coppa/")

# i-score API
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/i-score/base/lib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/i-score/base/plugins/iscore-plugin-deviceexplorer")

file(GLOB_RECURSE HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} ${SRCS} ${HDRS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr cxx_noexcept cxx_variadic_templates)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
target_link_libraries(${PROJECT_NAME}
                        Qt5::Core Qt5::Widgets
                        iscore_lib_base iscore_plugin_deviceexplorer)


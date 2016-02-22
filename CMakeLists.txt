cmake_minimum_required(VERSION 3.2)
project(iscore_addon_coppa)

iscore_common_setup()

add_subdirectory(coppa)

# Packages
find_package(Qt5 5.5 REQUIRED COMPONENTS Core Widgets Network)
find_package(KF5DNSSD NO_MODULE)

file(GLOB_RECURSE HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB OSSIA_WRAPPER_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/ossia_wrapper/src/*.cpp"
    )

add_library(APICoppa ${OSSIA_WRAPPER_SRCS})
target_link_libraries(APICoppa  PUBLIC coppa OSSIAHeadersLib)
target_include_directories(APICoppa PUBLIC "${API_HEADERS_FOLDER}")

add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} PUBLIC
                      Qt5::Core Qt5::Widgets
                      iscore_lib_base iscore_plugin_deviceexplorer
                      iscore_plugin_ossia iscore_addon_network
                      APICoppa KF5DNSSD)

setup_iscore_plugin(${PROJECT_NAME})

cmake_minimum_required(VERSION 3.2)
project(iscore_plugin_coppa)

add_subdirectory(coppa)
add_subdirectory(i-score i-score-build)

# Should make a property for i-score plug-ins
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0042 NEW)

# Packages
find_package(Qt5 5.5 REQUIRED COMPONENTS Core Widgets)

# For coppa it's in set_coppa_properties

# i-score API
include_directories("${CMAKE_CURRENT_BINARY_DIR}/i-score-build")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/i-score/base/lib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/i-score/base/plugins/iscore-plugin-deviceexplorer")

file(GLOB_RECURSE HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


file(GLOB OSSIA_WRAPPER_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/ossia_wrapper/src/*.cpp"
    )

add_library(APICoppa SHARED ${OSSIA_WRAPPER_SRCS})
target_link_libraries(APICoppa coppa OSSIAHeadersLib)
target_include_directories(APICoppa PUBLIC "${API_HEADERS_FOLDER}")



add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME}
                        Qt5::Core Qt5::Widgets
                        iscore_lib_base iscore_plugin_deviceexplorer iscore_plugin_ossia APICoppa)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION plugins/
        ARCHIVE DESTINATION static_plugins/)

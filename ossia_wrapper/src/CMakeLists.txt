cmake_minimum_required(VERSION 2.8)

project(APICoppaImplementation)

add_subdirectory(coppa)
add_subdirectory(API)

file(GLOB SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )

add_library(APICoppa ${SRCS})
target_link_libraries(APICoppa coppa OSSIAHeadersLib)
target_include_directories(APICoppa PUBLIC "${API_HEADERS_FOLDER}")


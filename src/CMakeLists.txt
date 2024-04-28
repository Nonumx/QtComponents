cmake_minimum_required(VERSION 3.25)
project(pc_widgets)

set(CMAKE_PREFIX_PATH $ENV{Qt5Home})
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SRC_FILES
        ${PROJECT_SOURCE_DIR}/*.h
        ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} resources.qrc)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE PC_WIDGETS_LIB)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)

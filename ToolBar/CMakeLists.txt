cmake_minimum_required(VERSION 3.16)

project(ToolBar)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH $ENV{Qt5_HOME}/lib/cmake)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} main.cpp tool_bar.cpp tool_bar.h tool_bar.ui resources.qrc)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}
)
cmake_minimum_required(VERSION 3.16)

project(EmbeddedWidget)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH $ENV{Qt5_HOME}/lib/cmake)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} main.cpp embedded_widget.h embedded_widget.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets toml11::toml11)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}
)
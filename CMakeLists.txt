cmake_minimum_required(VERSION 3.16)

project(QtComponents)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ToolBar)
add_subdirectory(ProjectExplorer)
add_subdirectory(EmbeddedWidget)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rd_party/toml11)

cmake_minimum_required(VERSION 3.22)
project(BoundedString
        LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE
  ${PROJECT_NAME}.hpp
)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

add_executable(${PROJECT_NAME}_test test.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE
  ${PROJECT_NAME}
)

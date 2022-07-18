cmake_minimum_required(VERSION 3.16)
project(BoundedString
  DESCRIPTION "A bounded string implementation"
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

option(BUILD_DOC "Build documentation" ON)
if(BUILD_DOC)
  find_package(Doxygen REQUIRED dot)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)

  doxygen_add_docs(doc_${PROJECT_NAME}
    ${PROJECT_NAME}.hpp README.md
    ALL
    COMMENT "Generate ${PROJECT_NAME} API documentation")
else()
  message("Please install Doxygen to generate API documentation")
endif()

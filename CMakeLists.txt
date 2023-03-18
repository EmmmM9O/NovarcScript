cmake_minimum_required(VERSION 3.0.0)
project(NovarcScript VERSION 0.1.0)
add_compile_options(-std=c++2a)
set(CMAKE_CXX_STANDARD 20)
set(BUILD_DIR "${PROJECT_SOURCE_DIR}/bin")

set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
include_directories(${PROJECT_SOURCE_DIR})
add_executable(NovarcScript "./src/main.cpp")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME}
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	POST_BUILD
		"${PROJECT_SOURCE_DIR}/src/assets"
		"$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"
)

cmake_minimum_required(VERSION 2.6)

if (CMAKE_VERSION VERSION_LESS "3.1")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
	endif ()
else ()
	set (CMAKE_CXX_STANDARD 14)
endif ()

project(FourierTransform)

add_executable(FourierTransform 
	src/main.cpp
	)

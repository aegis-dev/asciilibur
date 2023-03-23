cmake_minimum_required (VERSION 3.8)

project("asciilibur")

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE ASCIILIBUR_SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

set(ASCIILIBUR_INCLUDES
	${CMAKE_CURRENT_SOURCE_DIR}/
)

add_library(asciilibur STATIC ${ASCIILIBUR_SOURCE})
target_include_directories(asciilibur PUBLIC ${ASCIILIBUR_INCLUDES})

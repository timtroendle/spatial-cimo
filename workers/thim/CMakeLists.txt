cmake_minimum_required (VERSION 3.0)
project (Thim)

include_directories("${PROJECT_SOURCE_DIR}/worker_sdk/include")
include_directories("${PROJECT_SOURCE_DIR}/generated")

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif()

find_library(LIB_WORKER_SDK WorkerSdk "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_PROTO NAMES protobuf libprotobuf PATHS "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_SSL ssl "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_RAKNET RakNetLibStatic "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)

FILE(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cc")

# Build the worker
add_executable(Thim ${SOURCES})
target_link_libraries(Thim ${LIB_WORKER_SDK} ${LIB_PROTO} ${LIB_SSL} ${LIB_RAKNET} ${CMAKE_DL_LIBS})

# Create the Worker@OS.zip file
set(WORKER_ASSEMBLY_DIR "${PROJECT_SOURCE_DIR}/../../build/assembly/worker")
file(MAKE_DIRECTORY ${WORKER_ASSEMBLY_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(WORKER_NAME "Thim@Mac")
	set(TARGET_NAME "${WORKER_NAME}")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(WORKER_NAME "Thim@${CMAKE_SYSTEM_NAME}")
	set(TARGET_NAME "${WORKER_NAME}.exe")
else()
	set(WORKER_NAME "Thim@${CMAKE_SYSTEM_NAME}")
	set(TARGET_NAME "${WORKER_NAME}")
endif()

add_custom_target(create_zip ALL
	WORKING_DIRECTORY "${WORKER_ASSEMBLY_DIR}"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Thim>" "."
	COMMAND ${CMAKE_COMMAND} -E rename "${WORKER_ASSEMBLY_DIR}/$<TARGET_FILE_NAME:Thim>" "${WORKER_ASSEMBLY_DIR}/${TARGET_NAME}"
	COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${WORKER_NAME}.zip" --format=zip "${TARGET_NAME}"
	COMMAND ${CMAKE_COMMAND} -E remove "${TARGET_NAME}"
	DEPENDS Thim)

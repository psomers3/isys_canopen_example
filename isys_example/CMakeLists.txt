cmake_minimum_required(VERSION 3.10 )
project(isys_example)
set (CMAKE_CXX_STANDARD 14)

# use pkg-config to get the libraries for linking
find_package(PkgConfig)
pkg_check_modules(LELY REQUIRED liblely-coapp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include_directories(include)
set(SRC_FILES src/CameraSlave.cpp)

add_executable(isys_example main.cpp ${SRC_FILES})
# link the library binaries
target_link_libraries(isys_example ${LELY_LIBRARIES})
target_link_libraries(isys_example Threads::Threads)
target_link_libraries(isys_example pthread)
target_link_libraries(isys_example ${OpenCV_LIBRARIES})

add_custom_command( TARGET isys_example POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/cpp-slave.eds"
                    $<TARGET_FILE_DIR:isys_example>
)
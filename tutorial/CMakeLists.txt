cmake_minimum_required(VERSION 3.10 )
project(tutorial)
set (CMAKE_CXX_STANDARD 14)

# use pkg-config to get the libraries for linking
find_package(PkgConfig)
pkg_check_modules(LELY REQUIRED liblely-coapp)

# define the executables
add_executable(master_tutorial master.cpp)
add_executable(slave_tutorial slave.cpp)

# link the library binaries
target_link_libraries(master_tutorial ${LELY_LIBRARIES})
target_link_libraries(slave_tutorial ${LELY_LIBRARIES})

# make the dcf file from the yaml definition and copy needed files to build directory
add_custom_command( COMMAND dcfgen -r cpp_tutorial.yml
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    TARGET master_tutorial POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/master.dcf"
                    $<TARGET_FILE_DIR:master_tutorial>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/cpp-slave.eds"
                    $<TARGET_FILE_DIR:master_tutorial>
)


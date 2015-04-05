project(l0-infra-test)

include_directories(${INFRA_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../test)

FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

sort_files(all_files)

add_executable(l0-infra-test  ${all_files})
target_link_libraries(l0-infra-test l0-infra magellan)


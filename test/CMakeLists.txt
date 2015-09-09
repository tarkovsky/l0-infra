project(l0-infra-test)

FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

add_executable(l0-infra-test  ${all_files})
target_link_libraries(l0-infra-test hamcrest magellan l0-infra)

FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

sort_files(all_files)

add_library(l0-infra STATIC ${all_files})
install(TARGETS l0-infra ARCHIVE DESTINATION lib)

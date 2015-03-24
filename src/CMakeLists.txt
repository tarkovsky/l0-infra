FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

sort_files(all_files)

add_library(infra STATIC ${all_files})
install(TARGETS infra ARCHIVE DESTINATION lib)

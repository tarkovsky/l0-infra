add_subdirectory(src)

install(DIRECTORY include/l0-infra DESTINATION include)

if(ENABLE_TEST)
  add_subdirectory(test)
endif()

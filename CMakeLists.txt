cmake_minimum_required(VERSION 3.16)
project(CppTemplate VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(lib_a)

add_executable(CppTemplate main.cpp)
target_link_libraries(CppTemplate libA)

if(BUILD_TESTS)

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(
    CppTemplateTest
    main.test.cpp
  )
  target_link_libraries(
    CppTemplateTest
    GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(CppTemplateTest)

endif()

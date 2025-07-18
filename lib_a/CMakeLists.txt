set(SOURCES
  HelloWorldClass.cpp
)

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")
add_library(libA SHARED ${SOURCES})

if(BUILD_TESTS)
  include(GoogleTest)
  enable_testing()
  set(TEST_SOURCES
    HelloWorldClass.test.cpp)

  add_executable(libA_Tests ${TEST_SOURCES})
  target_link_libraries(libA_Tests PRIVATE libA gtest_main)
  gtest_discover_tests(libA_Tests TEST_PREFIX libA)
endif()

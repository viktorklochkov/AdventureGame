#include "HelloWorldClass.hpp"

#include "gtest/gtest.h"

#include <string>

namespace lib_a {

TEST(HelloWorldClass, greeting) {
  EXPECT_EQ(greeting(), "Hello, World!\n");
}

}// namespace lib_a
#include <gtest/gtest.h>

// Including tests
#include "rtcsessiondescription.test.cc"
#include "rtcpeerconnection.test.cc"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

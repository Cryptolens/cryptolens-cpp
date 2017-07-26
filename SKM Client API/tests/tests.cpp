#include <iostream>

#include "gtest/gtest.h"

TEST(LicenseKeyAttributes, Mandatory) {
    EXPECT_EQ(123, 123);
}

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

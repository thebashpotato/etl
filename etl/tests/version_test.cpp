#include <etl.hpp>
#include <gtest/gtest.h>

TEST(EtlVersionTests, EnsureVersionStringIsCorrect)
{
    ASSERT_EQ(etl::VERSION_STRING, "0.8.0");
}

TEST(EtlVersionTests, EnsureVersionIntIsCorrect)
{
    ASSERT_EQ(etl::VERSION_MAJOR, 0);
    ASSERT_EQ(etl::VERSION_MINOR, 8);
    ASSERT_EQ(etl::VERSION_PATCH, 0);
}

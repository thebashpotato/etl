#include <etl.hpp>
#include <gtest/gtest.h>

TEST(EtlVersionTests, EnsureVersionStringIsCorrect)
{
    ASSERT_EQ(etl::VERSION_STRING, "0.7.0");
}

TEST(EtlVersionTests, EnsureVersionIntIsCorrect)
{
    ASSERT_EQ(etl::VERSION_MAJOR, 0);
    ASSERT_EQ(etl::VERSION_MINOR, 7);
    ASSERT_EQ(etl::VERSION_PATCH, 0);
}

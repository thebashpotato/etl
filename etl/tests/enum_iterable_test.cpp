#include <cstdint>
#include <etl.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(EtlIterable, EnumIterTest)
{
    enum class Integers : uint16_t
    {
        Zero = 0,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
    };

    using IntegerIterator = etl::EnumerationIterator<Integers, Integers::Zero, Integers::Nine>;

    std::vector<uint16_t> numbers;
    for (auto const &iter : IntegerIterator())
    {
        numbers.push_back(static_cast<uint16_t>(iter));
    }
    EXPECT_EQ(numbers.size(), 10);
}

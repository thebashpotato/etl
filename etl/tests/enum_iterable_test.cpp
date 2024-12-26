#include <cstdint>
#include <vector>

#include <etl.hpp>

#include <gtest/gtest.h>

TEST(EtlIterable, EnumIterTestStartIndexZero)
{
    enum class Integers : std::uint8_t
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

    std::vector<std::uint8_t> numbers;
    for (auto const &iter : IntegerIterator())
    {
        numbers.push_back(static_cast<std::uint8_t>(iter));
    }
    EXPECT_EQ(numbers.size(), 10);
    EXPECT_EQ(numbers.size(), IntegerIterator::size());
}

TEST(EtlIterable, EnumIterTestStartIndexOne)
{
    enum class Integers : std::uint8_t
    {
        One = 1,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
    };

    using IntegerIterator = etl::EnumerationIterator<Integers, Integers::One, Integers::Nine>;

    std::vector<std::uint8_t> numbers;
    for (auto const &iter : IntegerIterator())
    {
        numbers.push_back(static_cast<uint16_t>(iter));
    }

    EXPECT_EQ(numbers.size(), 9);
    EXPECT_EQ(numbers.size(), IntegerIterator::size());
}

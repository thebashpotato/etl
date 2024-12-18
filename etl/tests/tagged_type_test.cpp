#include <cstdint>
#include <etl.hpp>
#include <gtest/gtest.h>
#include <utility>

/// @brief Tagged type identifiers for a width and height.
/// Should not be used directly.
namespace tags
{

class WidthTag
{
};

class HeightTag
{
};

class ValueTag
{
};

} // namespace tags

using Width = etl::TaggedFundamental<tags::WidthTag, uint32_t>;
using Height = etl::TaggedFundamental<tags::HeightTag, uint32_t>;
using Value = etl::TaggedFundamental<tags::ValueTag, int32_t>;

class Rect
{
  public:
    Width width;
    Height height;

    Rect() noexcept = default;
    Rect(Width width, Height height) noexcept : width(std::move(width)), height(std::move(height))
    {
    }
};

TEST(EtlTaggedType, TaggedTypeTest)
{

    const Rect rect(Width(4), Height(2));

    ASSERT_EQ(rect.width, 4);
    ASSERT_EQ(rect.height, 2);
}

TEST(EtlTaggedType, ArithmeticOperators)
{
    Width w1(10);
    const Width w2(5);

    ASSERT_EQ((w1 + w2).value, 15);
    ASSERT_EQ((w1 - w2).value, 5);
    ASSERT_EQ((w1 * w2).value, 50);
    ASSERT_EQ((w1 / w2).value, 2);

    w1 += w2;
    ASSERT_EQ(w1.value, 15);

    w1 -= w2;
    ASSERT_EQ(w1.value, 10);

    w1 *= w2;
    ASSERT_EQ(w1.value, 50);

    w1 /= w2;
    ASSERT_EQ(w1.value, 10);
}

TEST(EtlTaggedType, BitwiseOperators)
{
    Value v1(0b1010);       // 10 in binary
    const Value v2(0b0110); // 6 in binary

    ASSERT_EQ((v1 & v2).value, 0b0010); // 2 in binary
    ASSERT_EQ((v1 | v2).value, 0b1110); // 14 in binary
    ASSERT_EQ((v1 ^ v2).value, 0b1100); // 12 in binary
    ASSERT_EQ((~v1).value, ~0b1010);

    v1 &= v2;
    ASSERT_EQ(v1.value, 0b0010); // 2 in binary

    v1 |= v2;
    ASSERT_EQ(v1.value, 0b0110); // 6 in binary

    v1 ^= v2;
    ASSERT_EQ(v1.value, 0b0000); // 0 in binary

    Value v3(0b0010); // 2 in binary
    const Value v4(1);

    ASSERT_EQ((v3 << v4).value, 0b0100); // 4 in binary
    ASSERT_EQ((v3 >> v4).value, 0b0001); // 1 in binary

    v3 <<= v4;
    ASSERT_EQ(v3.value, 0b0100); // 4 in binary

    v3 >>= v4;
    ASSERT_EQ(v3.value, 0b0010); // 2 in binary
}

TEST(EtlTaggedType, ComparisonOperators)
{
    const Width w1(10);
    const Width w2(5);

    ASSERT_TRUE(w1 > w2);
    ASSERT_TRUE(w1 >= w2);
    ASSERT_FALSE(w1 < w2);
    ASSERT_FALSE(w1 <= w2);
    ASSERT_FALSE(w1 == w2);
    ASSERT_TRUE(w1 != w2);

    const Width w3(10);
    ASSERT_TRUE(w1 == w3);
    ASSERT_FALSE(w1 != w3);
}

TEST(EtlTaggedType, ArithmeticWithFundamentalType)
{
    Width w1(10);
    const uint32_t val = 5;

    ASSERT_EQ((w1 + val).value, 15);
    ASSERT_EQ((w1 - val).value, 5);
    ASSERT_EQ((w1 * val).value, 50);
    ASSERT_EQ((w1 / val).value, 2);

    w1 += val;
    ASSERT_EQ(w1.value, 15);

    w1 -= val;
    ASSERT_EQ(w1.value, 10);

    w1 *= val;
    ASSERT_EQ(w1.value, 50);

    w1 /= val;
    ASSERT_EQ(w1.value, 10);
}

TEST(EtlTaggedType, BitwiseWithFundamentalType)
{
    Value v1(0b1010);           // 10 in binary
    const int32_t val = 0b0110; // 6 in binary

    ASSERT_EQ((v1 & val).value, 0b0010); // 2 in binary
    ASSERT_EQ((v1 | val).value, 0b1110); // 14 in binary
    ASSERT_EQ((v1 ^ val).value, 0b1100); // 12 in binary

    v1 &= val;
    ASSERT_EQ(v1.value, 0b0010); // 2 in binary

    v1 |= val;
    ASSERT_EQ(v1.value, 0b0110); // 6 in binary

    v1 ^= val;
    ASSERT_EQ(v1.value, 0b0000); // 0 in binary

    Value v3(0b0010); // 2 in binary
    const int32_t shift = 1;

    ASSERT_EQ((v3 << shift).value, 0b0100); // 4 in binary
    ASSERT_EQ((v3 >> shift).value, 0b0001); // 1 in binary

    v3 <<= shift;
    ASSERT_EQ(v3.value, 0b0100); // 4 in binary

    v3 >>= shift;
    ASSERT_EQ(v3.value, 0b0010); // 2 in binary
}

#include <cstdint>
#include <etl.hpp>
#include <gtest/gtest.h>

using namespace etl;


/**
 * @brief Tagged type identifiers for a width and height.
 * Should not be used directly.
 * */
namespace detail {
    class WidthTag {};
    class HeightTag {};
}// namespace detail

using Width = TaggedFundamental<detail::WidthTag, uint32_t>;
using Height = TaggedFundamental<detail::HeightTag, uint32_t>;


class Rect {
public:
    Width width_;
    Height height_;
    Rect(Width &&width, Height &&height) : width_(width), height_(height) {}
};


TEST(EtlTaggedType, TaggedTypeTest) {
    uint32_t width = 4;
    uint32_t height = 2;
    auto rect = Rect(Width(width), Height(height));
    ASSERT_EQ(rect.width_.get(), width);
    ASSERT_EQ(rect.height_.get(), height);
}

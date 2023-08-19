#include <etl.hpp>
#include <gtest/gtest.h>

using namespace etl;

auto divide(int numerator, int denominator) noexcept -> Result<int, Error> {
    if (denominator == 0) {
        return Result<int, Error>(Error::create("Division by zero Error"));
    }
    return Result<int, Error>(numerator / denominator);
}

TEST(EtlResult, ResultOkTypeTest) {
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    auto result = divide(numerator, denominator);
    ASSERT_TRUE(result.isOk());
    ASSERT_FALSE(result.isErr());
    ASSERT_EQ(result.ok().value(), 2);
}

TEST(EtlResult, ResultOkTypeTestDefaultConstructor) {
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    Result<int, Error> result{};

    result = divide(numerator, denominator);
    ASSERT_TRUE(result.isOk());
    ASSERT_FALSE(result.isErr());
    ASSERT_EQ(result.ok().value(), 2);
}

TEST(EtlResult, ResultOkTypeMapTest) {
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    auto result = divide(numerator, denominator);
    ASSERT_TRUE(result.isOk());
    ASSERT_FALSE(result.isErr());
    ASSERT_EQ(result.ok().value(), 2);

    Result<int, Error> squaredResult = result.map([](int square) -> int {
        return square * square;
    });

    ASSERT_TRUE(squaredResult.isOk());
    ASSERT_FALSE(squaredResult.isErr());
    ASSERT_EQ(squaredResult.ok().value(), 4);
}

TEST(EtlResult, ResultErrTypeTest) {
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;

    auto result = divide(numerator, denominator);
    ASSERT_TRUE(result.isErr());
    ASSERT_FALSE(result.isOk());
    ASSERT_EQ(result.err().value().msg(), "Division by zero Error");
}

TEST(EtlResult, ResultErrTypeMapErrTest) {
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;

    auto result = divide(numerator, denominator);
    ASSERT_TRUE(result.isErr());
    ASSERT_FALSE(result.isOk());
    ASSERT_EQ(result.err().value().msg(), "Division by zero Error");

    Result<int, Error> appendResult = result.mapErr([](Error error) -> Error {
        error.set("Error: Division by zero");
        return error;
    });

    ASSERT_TRUE(appendResult.isErr());
    ASSERT_FALSE(appendResult.isOk());
    ASSERT_EQ(appendResult.err().value().msg(), "Error: Division by zero");
}

TEST(EtlResult, ResultOkTypeUniquePtrSpecializationTest) {
    constexpr auto number = 42;
    Result<std::unique_ptr<int>, std::string> result(std::make_unique<int>(number));
    ASSERT_TRUE(result.isOk());
    ASSERT_FALSE(result.isErr());
    ASSERT_EQ(*result.ok().value(), number);
}

TEST(EtlResult, ResultErrTypeUniquePtrSpecializationTest) {
    Result<std::unique_ptr<int>, std::string> result("This is an error");
    ASSERT_TRUE(result.isErr());
    ASSERT_FALSE(result.isOk());
    ASSERT_EQ(result.err().value(), "This is an error");
}

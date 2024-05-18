#include <etl.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace etl;

auto divide(int numerator, int denominator) noexcept -> Result<int, Error>
{
    if (denominator == 0)
    {
        return Result<int, Error>(Error::create("Division by zero Error"));
    }
    return Result<int, Error>(numerator / denominator);
}

TEST(EtlResult, ResultOkTypeTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    const auto result = divide(numerator, denominator);
    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(result.ok().value(), 2);
}

TEST(EtlResult, ResultOkTypeTestDefaultConstructor)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    Result<int, Error> result;

    result = divide(numerator, denominator);
    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(result.ok().value(), 2);
}

TEST(EtlResult, ResultOkTypeMapTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;
    const auto result = divide(numerator, denominator);

    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(result.ok().value(), 2);

    const Result<int, Error> squared_result = result.map([](int square) -> int { return square * square; });

    ASSERT_TRUE(squared_result.is_ok());
    ASSERT_FALSE(squared_result.is_err());
    ASSERT_EQ(squared_result.ok().value(), 4);
}

TEST(EtlResult, ResultErrTypeTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;
    const auto result = divide(numerator, denominator);

    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.err().value().msg(), "Division by zero Error");
}

TEST(EtlResult, ResultErrTypeMapErrTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;
    const auto result = divide(numerator, denominator);

    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.err().value().msg(), "Division by zero Error");

    const Result<int, Error> appendResult = result.map_err([](Error error) -> Error {
        error.set("Error: Division by zero");
        return error;
    });

    ASSERT_TRUE(appendResult.is_err());
    ASSERT_FALSE(appendResult.is_ok());
    ASSERT_EQ(appendResult.err().value().msg(), "Error: Division by zero");
}

TEST(EtlResult, ResultOkTypeUniquePtrSpecializationTest)
{
    constexpr auto number = 42;
    const Result<std::unique_ptr<int>, std::string> result(std::make_unique<int>(number));

    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(*result.ok().value(), number);
}

TEST(EtlResult, ResultErrTypeUniquePtrSpecializationTest)
{
    const Result<std::unique_ptr<int>, std::string> result("This is an error");

    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.err().value(), "This is an error");
}

#include <cstdint>
#include <etl.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

/// @brief Create a basic Error class which inherits from the etl::BaseError
class GenericError : public etl::BaseError
{
  public:
    explicit GenericError(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    GenericError(const std::string_view &msg, const etl::detail::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

class OtherError : public etl::BaseError
{
  public:
    explicit OtherError(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    OtherError(const std::string_view &msg, const etl::detail::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

enum class ErrorTypes : std::uint8_t
{
    Generic,
    Other,
};

using namespace etl;

auto GetDynError(const ErrorTypes type) -> Result<Void, DynError>
{
    switch (type)
    {
    case ErrorTypes::Generic: {
        return Result<Void, DynError>(std::make_shared<GenericError>(GenericError("Generic Error")));
    }
    case ErrorTypes::Other: {
        return Result<Void, DynError>(std::make_shared<OtherError>(OtherError("Other Error")));
    }
    }
}

auto Divide(int numerator, int denominator) noexcept -> Result<int, GenericError>
{
    if (denominator == 0)
    {
        return Result<int, GenericError>(GenericError("Division by zero Error"));
    }
    return Result<int, GenericError>(numerator / denominator);
}

TEST(EtlResult, ResultOkTypeTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    const auto result = Divide(numerator, denominator);
    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(result.ok().value(), 2);
}

TEST(EtlResult, ResultOkTypeTestDefaultConstructor)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;

    Result<int, GenericError> result;

    result = Divide(numerator, denominator);
    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(result.ok().value(), 2);
}

TEST(EtlResult, ResultOkTypeMapTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 5;
    const auto result = Divide(numerator, denominator);

    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());
    ASSERT_EQ(result.ok().value(), 2);

    const Result<int, GenericError> squared_result = result.map([](int square) -> int { return square * square; });

    ASSERT_TRUE(squared_result.is_ok());
    ASSERT_FALSE(squared_result.is_err());
    ASSERT_EQ(squared_result.ok().value(), 4);
}

TEST(EtlResult, ResultErrTypeTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;
    const auto result = Divide(numerator, denominator);

    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.err().value().msg(), "Division by zero Error");
}

TEST(EtlResult, ResultErrTypeMapErrTest)
{
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;
    const auto result = Divide(numerator, denominator);

    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.err().value().msg(), "Division by zero Error");

    const Result<int, GenericError> append_result = result.map_err([](GenericError error) -> GenericError {
        error.set_msg("Error: Division by zero");
        return error;
    });

    ASSERT_TRUE(append_result.is_err());
    ASSERT_FALSE(append_result.is_ok());
    ASSERT_EQ(append_result.err().value().msg(), "Error: Division by zero");
}

TEST(EtlResult, ResultOkTypeUniquePtrSpecializationTest)
{
    constexpr auto number = 42;
    Result<std::unique_ptr<int>, std::string> result(std::make_unique<int>(number));

    ASSERT_TRUE(result.is_ok());
    ASSERT_FALSE(result.is_err());

    const std::unique_ptr<int> value = std::move(*result.ok());
    ASSERT_EQ(*value, number);
}

TEST(EtlResult, ResultErrTypeUniquePtrSpecializationTest)
{
    const Result<std::unique_ptr<int>, GenericError> result(GenericError("This is an error"));

    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());

    const GenericError err = std::move(*result.err());

    ASSERT_EQ(err.msg(), "This is an error");
}

TEST(EtlResult, DynErrorGeneric)
{
    auto result = GetDynError(ErrorTypes::Generic);
    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    auto error = result.err().value();
    ASSERT_EQ(error->msg(), "Generic Error");
}

TEST(EtlResult, DynErrorOther)
{
    auto result = GetDynError(ErrorTypes::Other);
    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());
    auto error = *result.err();
    ASSERT_EQ(error->msg(), "Other Error");
}

TEST(EtlResult, DynErrorDynamicPointerCastGeneric)
{
    auto result = GetDynError(ErrorTypes::Generic);
    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());

    // Check if the error is of type GenericError, if it is the
    // resulting `error` will be not be nullptr, and will be the GenericError type.
    // This is how you can check which error was ultimately returned.
    // This simulates Rust's chaining error return pattern (some what)
    auto error = std::dynamic_pointer_cast<GenericError>(*result.err());
    ASSERT_TRUE(error != nullptr);
    ASSERT_EQ(error->msg(), "Generic Error");
}

TEST(EtlResult, DynErrorDynamicPointerCastOther)
{
    auto result = GetDynError(ErrorTypes::Other);
    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());

    // Check if the error is of type OtherError, if it is the
    // resulting `error` will be not be nullptr, and will be the OtherError type.
    // This is how you can check which error was ultimately returned.
    // This simulates Rust's chaining error return pattern (some what)
    auto error = std::dynamic_pointer_cast<OtherError>(*result.err());
    ASSERT_TRUE(error != nullptr);
    ASSERT_EQ(error->msg(), "Other Error");
}

TEST(EtlResult, DynErrorDynamicPointerCastOtherToGeneric)
{
    auto result = GetDynError(ErrorTypes::Other);
    ASSERT_TRUE(result.is_err());
    ASSERT_FALSE(result.is_ok());

    // Check if the error is of type GenericError, if it isn't the
    // resulting `error` will be nullptr. This is how you can check which error
    // was ultimately returned. This simulated Rust's chaining error return pattern (some what)
    auto error = std::dynamic_pointer_cast<GenericError>(*result.err());
    ASSERT_TRUE(error == nullptr);
}

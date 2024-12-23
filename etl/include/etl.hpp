/// Copyright (c) 2024, Matt Williams
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
/// following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
/// disclaimer.
///
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
/// following disclaimer in the documentation and/or other materials provided with the distribution.
///
/// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
/// products derived from this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
/// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// Original Source: https://github.com/thebashpotato/extra-template-library

#pragma once

#if __cplusplus >= 201702L

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace etl
{

constexpr auto VERSION_MAJOR = 0;
constexpr auto VERSION_MINOR = 8;
constexpr auto VERSION_PATCH = 1;
constexpr auto VERSION = (VERSION_MAJOR * 10000) + (VERSION_MINOR * 100) + VERSION_PATCH;
constexpr std::string_view VERSION_STRING = "0.8.1";

/// @brief Ditch those old C style for loops and iterate over your enums safely with ranged for loops.
///
/// @details Currently assumes the enumeration is contiguous (no gaps).
///
/// @example tests/enum_iterable_test.cpp
template <typename EnumIterable, EnumIterable beginValue, EnumIterable endValue> class EnumerationIterator
{
  public:
    /// @brief Default constructor builds an instance to the first value
    /// in the enumeration.
    ///
    /// @details Used in the begin() method.
    EnumerationIterator() noexcept : m_value(static_cast<value_t>(beginValue))
    {
    }

    /// @brief Constructs an instance to a specified value.
    ///
    /// @details Used in the end() method.
    explicit EnumerationIterator(EnumIterable const &iter) noexcept : m_value(static_cast<value_t>(iter))
    {
    }

    /// @brief Default Destructor Move/Copy constructor and assignment
    virtual ~EnumerationIterator() = default;
    EnumerationIterator(EnumerationIterator &&rhs) noexcept = default;
    auto operator=(EnumerationIterator &&rhs) noexcept -> EnumerationIterator & = default;
    EnumerationIterator(const EnumerationIterator &rhs) = default;
    auto operator=(const EnumerationIterator &rhs) -> EnumerationIterator & = default;

  public:
    /// @brief ++this overload
    ///
    /// @details Increments the underlying value and then returns
    /// an instance of itself. this++ not implemented, as it is
    /// ineffecient and usually not needed.
    [[maybe_unused]] auto operator++() noexcept -> EnumerationIterator
    {
        ++this->m_value;
        return *this;
    }

    /// @brief Dereference overload
    ///
    /// @details Gets an instance to the current underlying value
    /// after casting it the type EnumIterable.
    [[nodiscard]] auto operator*() noexcept -> EnumIterable
    {
        return static_cast<EnumIterable>(m_value);
    }

    /// @brief Is equal overload
    [[nodiscard]] auto operator==(const EnumerationIterator &rhs_iterator) const noexcept -> bool
    {
        return m_value == rhs_iterator.m_value;
    }

    /// @brief Not equal overload
    [[nodiscard]] auto operator!=(const EnumerationIterator &rhs_iterator) const noexcept -> bool
    {
        return !(*this == rhs_iterator);
    }

  public:
    /// @brief Return the beginning value, this will use the default constructor.
    [[nodiscard]] auto begin() const noexcept -> EnumerationIterator
    {
        return *this;
    }

    /// @brief Return the end value.
    [[nodiscard]] auto end() noexcept -> EnumerationIterator
    {
        // cache the value
        static const auto endIter = ++EnumerationIterator(endValue);
        return endIter;
    }

  private:
    /// @brief Verifys the type is indeed an enumeration class.
    ///
    /// @link https://en.cppreference.com/w/cpp/types/underlying_type
    using value_t = std::underlying_type_t<EnumIterable>;
    std::int64_t m_value;
};

/// @brief Tag a primitive fundamental type to descriptive class names.
///
/// @details Solves the issue when a function takes in many arguments of the same type,
/// which can lead to confusion and mistakes from the programmer.
/// For example consider a Rectangle with a constructor that takes a uint32_t width, and uint32_t height.
/// The width and the height can easily be swapped, resulting in runtime bugs.
/// This can be avoided by "Tagging the type".
///
/// @example tests/tagged_type_test.cpp
template <typename Tag, typename FundamentalType> class TaggedFundamental
{
  public:
    FundamentalType value{};

  public:
    /// @brief All the constructors needed to build a fundamental wrapped type
    TaggedFundamental()
    {
        static_assert(std::is_fundamental<FundamentalType>::value);
    };

    constexpr explicit TaggedFundamental(FundamentalType v) noexcept : value(std::move(v))
    {
        static_assert(std::is_fundamental<FundamentalType>::value);
    }

    virtual ~TaggedFundamental() = default;
    TaggedFundamental(TaggedFundamental &&rhs) noexcept = default;
    auto operator=(TaggedFundamental &&rhs) noexcept -> TaggedFundamental & = default;
    TaggedFundamental(const TaggedFundamental &rhs) = default;
    auto operator=(const TaggedFundamental &rhs) -> TaggedFundamental & = default;

  public:
    //////////////////////////////////
    /// Arithmetic operator overloads
    //////////////////////////////////
    auto operator+(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value + rhs.value);
    }

    auto operator+(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value + rhs);
    }

    auto operator-(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value - rhs.value);
    }

    auto operator-(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value - rhs);
    }

    auto operator*(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value * rhs.value);
    }

    auto operator*(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value * rhs);
    }

    auto operator/(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value / rhs.value);
    }

    auto operator/(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value / rhs);
    }

    //////////////////////////////////////////////////////
    /// Compound assignment arithmetic operator overloads
    /////////////////////////////////////////////////////
    auto operator+=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value += rhs.value;
        return *this;
    }

    auto operator+=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value += rhs;
        return *this;
    }

    auto operator-=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value -= rhs.value;
        return *this;
    }

    auto operator-=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value -= rhs;
        return *this;
    }

    auto operator*=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value *= rhs.value;
        return *this;
    }

    auto operator*=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value *= rhs;
        return *this;
    }

    auto operator/=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value /= rhs.value;
        return *this;
    }

    auto operator/=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value /= rhs;
        return *this;
    }

    //////////////////////////////////
    /// Comparison operator overloads
    //////////////////////////////////
    auto operator<(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value < rhs.value;
    }

    auto operator<(const FundamentalType &rhs) const noexcept -> bool
    {
        return value < rhs;
    }

    auto operator<=(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value <= rhs.value;
    }

    auto operator<=(const FundamentalType &rhs) const noexcept -> bool
    {
        return value <= rhs;
    }

    auto operator>(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value > rhs.value;
    }

    auto operator>(const FundamentalType &rhs) const noexcept -> bool
    {
        return value > rhs;
    }

    auto operator>=(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value >= rhs.value;
    }

    auto operator>=(const FundamentalType &rhs) const noexcept -> bool
    {
        return value >= rhs;
    }

    auto operator==(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value == rhs.value;
    }

    auto operator==(const FundamentalType &rhs) const noexcept -> bool
    {
        return value == rhs;
    }

    auto operator!=(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value != rhs.value;
    }

    auto operator!=(const FundamentalType &rhs) const noexcept -> bool
    {
        return value != rhs;
    }

    ////////////////////////////////
    /// Bitwise operator overloads
    ///////////////////////////////
    auto operator&(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value & rhs.value);
    }

    auto operator&(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value & rhs);
    }

    auto operator|(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value | rhs.value);
    }

    auto operator|(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value | rhs);
    }

    auto operator^(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value ^ rhs.value);
    }

    auto operator^(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value ^ rhs);
    }

    auto operator~() const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(~value);
    }

    auto operator<<(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value << rhs.value);
    }

    auto operator<<(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value << rhs);
    }

    auto operator>>(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value >> rhs.value);
    }

    auto operator>>(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value >> rhs);
    }

    ///////////////////////////////////////////////////
    /// Compound assignment bitwise operator overloads
    ///////////////////////////////////////////////////
    auto operator&=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value &= rhs.value;
        return *this;
    }

    auto operator&=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value &= rhs;
        return *this;
    }

    auto operator|=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value |= rhs.value;
        return *this;
    }

    auto operator|=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value |= rhs;
        return *this;
    }

    auto operator^=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value ^= rhs.value;
        return *this;
    }

    auto operator^=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value ^= rhs;
        return *this;
    }

    auto operator<<=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value <<= rhs.value;
        return *this;
    }

    auto operator<<=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value <<= rhs;
        return *this;
    }

    auto operator>>=(const TaggedFundamental &rhs) noexcept -> TaggedFundamental &
    {
        value >>= rhs.value;
        return *this;
    }

    auto operator>>=(const FundamentalType &rhs) noexcept -> TaggedFundamental &
    {
        value >>= rhs;
        return *this;
    }
};

/// @brief Holds useful runtime source code location information for use in Errors.
///
/// @details Should not be used directly, rather the user should pass the `etl::RUNTIME_INFO`
/// macro invocation.
class SourceCodeLocation
{
  public:
    SourceCodeLocation() = delete;

    /// @brief The only valid constructor
    ///
    /// @param `file` name of the file where the error occurred
    /// @param `line` line number where the error occurred
    /// @param `func` name of the function where the error occurred
    SourceCodeLocation(const std::string_view &file, const uint32_t line, const std::string_view &func) noexcept
        : m_file(file), m_line(line), m_func(func)
    {
    }

    /// @brief Default Destructor, Move/Copy constructor and assignment
    virtual ~SourceCodeLocation() = default;
    SourceCodeLocation(SourceCodeLocation &&rhs) noexcept = default;
    auto operator=(SourceCodeLocation &&rhs) noexcept -> SourceCodeLocation & = default;
    SourceCodeLocation(const SourceCodeLocation &rhs) = default;
    auto operator=(const SourceCodeLocation &rhs) -> SourceCodeLocation & = default;

  public:
    /// @brief Get the file name in which the error occured
    [[nodiscard]] auto file() const noexcept -> std::string_view
    {
        return m_file;
    }

    /// @brief Get the line number in which the error occured
    [[nodiscard]] auto line() const noexcept -> uint32_t
    {
        return m_line;
    }

    /// @brief Get the name of the function in which the error occured
    [[nodiscard]] auto function() const noexcept -> std::string_view
    {
        return m_func;
    }

  private:
    std::string m_file;
    uint32_t m_line;
    std::string m_func;
};

/// @brief Wrapper macro which constructs an instance of SourceCodeLocation in-place
/// and guarantees that the accurate file, function name, and line number will be
/// reported.
#ifdef __linux__
#define RUNTIME_INFO SourceCodeLocation(__FILE__, __LINE__, static_cast<const char *>(__PRETTY_FUNCTION__))
#else
#define RUNTIME_INFO SourceCodeLocation(__FILE__, __LINE__, static_cast<const char *>(__func__))
#endif

/// @brief Base Abstract Error for use with the Result and DynError types.
///
/// @detail Contains Function File and line information
///
/// @example `etl/tests/result_test.cpp` to see an example of using this BaseError class
class BaseError
{
  public:
    /// @brief Constructs the error with only a message
    explicit BaseError(const std::string_view &msg) noexcept : m_msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    BaseError(const std::string_view &msg, const SourceCodeLocation &slc) noexcept : m_msg(msg)
    {
        m_info.append("Error: ")
            .append(msg)
            .append("\nFunction: ")
            .append(slc.function())
            .append("\nFile: ")
            .append(slc.file())
            .append(":")
            .append(std::to_string(slc.line()));
    }

    BaseError() = default;
    virtual ~BaseError() = default;
    BaseError(BaseError &&rhs) noexcept = default;
    BaseError(const BaseError &rhs) = default;

  public:
    auto operator=(BaseError &&rhs) noexcept -> BaseError & = default;
    auto operator=(const BaseError &rhs) -> BaseError & = default;

  public:
    [[nodiscard]] virtual auto msg() const noexcept -> std::string
    {
        return m_msg;
    };

    void virtual set_msg(std::string new_msg) noexcept
    {
        m_msg = std::move(new_msg);
    }

    [[nodiscard]] virtual auto info() const noexcept -> std::string
    {
        if (!m_info.empty())
        {
            return m_info;
        }
        return m_msg;
    }

  protected:
    std::string m_msg;
    std::string m_info;
};

/// @brief Polymorphic error type definition
using DynError = std::shared_ptr<etl::BaseError>;

/// @brief Empty stub type for when the user wants a result with an Ok type
/// with no value, since c++ doesn't have rusts () type, this is my workaround.
///
/// @details In Rust it would be Result<(), String>.
/// Converted into this C++ class would be, Result<Void, std::string>.
class Void
{
};

/// @brief Generic Result type modeled after the Rust lanaguage's Result<T, E>
template <typename OkType, typename ErrType> class Result
{
  public:
    /// @brief All the constructors needed to build an OkType or ErrType
    Result() noexcept = default;
    explicit Result(const OkType &value) noexcept : m_result(value), m_is_ok(true)
    {
    }

    explicit Result(OkType &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : m_result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : m_result(std::move(error))
    {
    }

    /// @brief Default Destructor, Move/Copy constructor and assignment
    virtual ~Result() = default;
    Result(Result &&rhs) noexcept = default;
    auto operator=(Result &&rhs) noexcept -> Result & = default;
    Result(const Result &rhs) = default;
    auto operator=(const Result &rhs) -> Result & = default;

  public:
    /// @brief Check if the variant is of the [OkType]
    [[nodiscard]] auto is_ok() const noexcept -> bool
    {
        return m_is_ok;
    }

    /// @brief Check if the variant is of the [ErrType]
    [[nodiscard]] auto is_err() const noexcept -> bool
    {
        return !m_is_ok;
    }

    /// @brief Get the OkType value
    ///
    /// @details The use should always use is_ok() before using ok()
    ///
    /// @return std::optinal<OkType> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() const noexcept -> std::optional<OkType>
    {
        std::optional<OkType> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<OkType>(&m_result))
            {
                ret.emplace(*value);
            }
        }
        return ret;
    }

    /// @brief Get the ErrType value
    ///
    /// @details The use should always use is_err() before using err()
    ///
    /// @return std::optinal<ErrType> for safety, incase the user did not call
    /// is_err() before using this method.
    [[nodiscard]] auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!m_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&m_result))
            {
                ret.emplace(*err);
            }
        }
        return ret;
    }

    /// @brief Maps a custom/lambda function to the [OkType] leaving the [ErrType] untouched.
    ///
    /// @details The use should always use is_ok() before using map()
    ///
    /// @return Returns the Result type with a modified OkType, or just the Result unmodified
    /// if is_ok() is false.
    template <typename Function> [[nodiscard]] auto map(Function &&func) const noexcept -> Result<OkType, ErrType>
    {
        if (m_is_ok)
        {
            if constexpr (std::is_invocable_r_v<OkType, Function, const OkType &>)
            {
                return Result<OkType, ErrType>(
                    std::invoke(std::forward<Function>(func), *std::get_if<OkType>(&m_result)));
            }
            else
            {
                return Result<OkType, ErrType>(*std::get_if<OkType>(&m_result));
            }
        }
        return Result<OkType, ErrType>(std::move(*std::get_if<ErrType>(&m_result)));
    }

    /// @brief Maps a custom/lambda function to the [ErrType] leaving the [OkType] untouched.
    ///
    /// @details The use should always use the is_err() method before using mapErr().
    ///
    /// @return Returns the Result type with a modified ErrType, or just the Result unmodified
    /// if is_err() is false.
    template <typename Function> [[nodiscard]] auto map_err(Function &&func) const noexcept -> Result<OkType, ErrType>
    {
        if (!m_is_ok)
        {
            if constexpr (std::is_invocable_r_v<ErrType, Function, const ErrType &>)
            {
                return Result<OkType, ErrType>(
                    std::invoke(std::forward<Function>(func), *std::get_if<ErrType>(&m_result)));
            }
            else
            {
                return Result<OkType, ErrType>(*std::get_if<ErrType>(&m_result));
            }
        }
        return Result<OkType, ErrType>(std::move(*std::get_if<OkType>(&m_result)));
    }

  private:
    std::variant<OkType, ErrType> m_result;
    bool m_is_ok{};
};

/// @brief Result Template Specialization for std::unique_ptr<OkType>.
///
/// @details Since std::unique_ptr is a move only type, the generic Result implementation
/// confuses which constructors are to be used and its not enough. Objects with deleted copy constructors
/// will need a templated specialization. You can use this specialiation for your custom type as
/// an example on how to do it.
template <typename OkType, typename ErrType> class Result<std::unique_ptr<OkType>, ErrType>
{

  public:
    /// @brief All the constructors needed to build an OkType or ErrType for a move only type
    Result() noexcept = default;
    explicit Result(std::unique_ptr<OkType> &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : m_result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : m_result(std::move(error))
    {
    }

  public:
    /// @brief Check if the variant value is of the [OkType]
    [[nodiscard]] auto is_ok() const noexcept -> bool
    {
        return m_is_ok;
    }

    /// @brief Check if the variant value is of the [ErrType]
    [[nodiscard]] auto is_err() const noexcept -> bool
    {
        return !m_is_ok;
    }

    /// @brief Gets the [OkType] value from the variant
    ///
    /// @details The user should always use the is_ok() method before using ok()
    ///
    /// @return std::optional<std::unique_ptr<OkType>> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<std::unique_ptr<OkType>>
    {
        std::optional<std::unique_ptr<OkType>> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<std::unique_ptr<OkType>>(&m_result))
            {
                ret.emplace(std::move(std::make_unique<OkType>(**value)));
            }
        }
        return ret;
    }

    /// @brief Get the [ErrType] value from the variant
    ///
    /// @details The user should always use the is_err() method before using err()
    ///
    /// @return std::optinal<ErrType> for safety, incase the user did not call
    /// is_err() before using this method.
    [[nodiscard]] auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!m_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&m_result))
            {
                ret.emplace(*err);
            }
        }
        return ret;
    }

  private:
    std::variant<std::unique_ptr<OkType>, ErrType> m_result;
    bool m_is_ok{};
};

} // namespace etl

#endif // __cplusplus >= 201702l

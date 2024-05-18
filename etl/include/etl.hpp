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

constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 7;
constexpr int VERSION_PATCH = 0;
constexpr int VERSION = (VERSION_MAJOR * 10000) + (VERSION_MINOR * 100) + VERSION_PATCH;
constexpr std::string_view VERSION_STRING = "0.7.0";

/// @brief Ditch those old C style for loops and iterate over your enums safely with ranged for loops.
///
/// @details Currently assumes the enumeration is contiguous (no gaps).
///
/// @example tests/enum_iterable_test.cpp
template <typename EnumIterable, EnumIterable beginValue, EnumIterable endValue> class EnumerationIterator
{
  private:
    /// @brief Verifys the type is indeed an enumeration class.
    ///
    /// @link https://en.cppreference.com/w/cpp/types/underlying_type
    using value_t = std::underlying_type_t<EnumIterable>;
    std::int64_t _value;

  public:
    /// @brief Default constructor builds an instance to the first value
    /// in the enumeration.
    ///
    /// @details Used in the begin() method.
    EnumerationIterator() noexcept : _value(static_cast<value_t>(beginValue))
    {
    }

    /// @brief Constructs an instance to a specified value.
    ///
    /// @details Used in the end() method.
    explicit EnumerationIterator(EnumIterable const &iter) noexcept : _value(static_cast<value_t>(iter))
    {
    }

    /// @brief Default Destructor Move/Copy constructor and assignment
    virtual ~EnumerationIterator() = default;
    EnumerationIterator(EnumerationIterator &&other) noexcept = default;
    auto operator=(EnumerationIterator &&other) noexcept -> EnumerationIterator & = default;
    EnumerationIterator(EnumerationIterator const &other) = default;
    auto operator=(EnumerationIterator const &other) -> EnumerationIterator & = default;

  public:
    /// @brief ++this overload
    ///
    /// @details Increments the underlying value and then returns
    /// an instance of itself. this++ not implemented, as it is
    /// ineffecient and usually not needed.
    [[maybe_unused]] auto operator++() noexcept -> EnumerationIterator
    {
        ++this->_value;
        return *this;
    }

    /// @brief Dereference overload
    ///
    /// @details Gets an instance to the current underlying value
    /// after casting it the type EnumIterable.
    [[nodiscard]] auto operator*() noexcept -> EnumIterable
    {
        return static_cast<EnumIterable>(_value);
    }

    /// @brief Is equal overload
    [[nodiscard]] auto operator==(EnumerationIterator const &other_iterator) const noexcept -> bool
    {
        return _value == other_iterator._value;
    }

    /// @brief Not equal overload
    [[nodiscard]] auto operator!=(EnumerationIterator const &other_iterator) const noexcept -> bool
    {
        return !(*this == other_iterator);
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

    constexpr explicit TaggedFundamental(FundamentalType value) noexcept : value(std::move(value))
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
    auto operator+(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value + rhs.value);
    }

    auto operator+(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value + rhs);
    }

    auto operator-(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value - rhs.value);
    }

    auto operator-(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value - rhs);
    }

    auto operator*(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value * rhs.value);
    }

    auto operator*(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value * rhs);
    }

    auto operator/(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value / rhs.value);
    }

    auto operator/(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value / rhs);
    }

    //////////////////////////////////////////////////////
    /// Compound assignment arithmetic operator overloads
    /////////////////////////////////////////////////////
    auto operator+=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value += rhs.value;
        return *this;
    }

    auto operator+=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value += rhs;
        return *this;
    }

    auto operator-=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value -= rhs.value;
        return *this;
    }

    auto operator-=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value -= rhs;
        return *this;
    }

    auto operator*=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value *= rhs.value;
        return *this;
    }

    auto operator*=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value *= rhs;
        return *this;
    }

    auto operator/=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value /= rhs.value;
        return *this;
    }

    auto operator/=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value /= rhs;
        return *this;
    }

    //////////////////////////////////
    /// Comparison operator overloads
    //////////////////////////////////
    auto operator<(TaggedFundamental const &rhs) const noexcept -> bool
    {
        return value < rhs.value;
    }

    auto operator<(FundamentalType const &rhs) const noexcept -> bool
    {
        return value < rhs;
    }

    auto operator<=(TaggedFundamental const &rhs) const noexcept -> bool
    {
        return value <= rhs.value;
    }

    auto operator<=(FundamentalType const &rhs) const noexcept -> bool
    {
        return value <= rhs;
    }

    auto operator>(TaggedFundamental const &rhs) const noexcept -> bool
    {
        return value > rhs.value;
    }

    auto operator>(FundamentalType const &rhs) const noexcept -> bool
    {
        return value > rhs;
    }

    auto operator>=(TaggedFundamental const &rhs) const noexcept -> bool
    {
        return value >= rhs.value;
    }

    auto operator>=(FundamentalType const &rhs) const noexcept -> bool
    {
        return value >= rhs;
    }

    auto operator==(TaggedFundamental const &rhs) const noexcept -> bool
    {
        return value == rhs.value;
    }

    auto operator==(FundamentalType const &rhs) const noexcept -> bool
    {
        return value == rhs;
    }

    auto operator!=(TaggedFundamental const &rhs) const noexcept -> bool
    {
        return value != rhs.value;
    }

    auto operator!=(FundamentalType const &rhs) const noexcept -> bool
    {
        return value != rhs;
    }

    ////////////////////////////////
    /// Bitwise operator overloads
    ///////////////////////////////
    auto operator&(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value & rhs.value);
    }

    auto operator&(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value & rhs);
    }

    auto operator|(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value | rhs.value);
    }

    auto operator|(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value | rhs);
    }

    auto operator^(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value ^ rhs.value);
    }

    auto operator^(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value ^ rhs);
    }

    auto operator~() const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(~value);
    }

    auto operator<<(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value << rhs.value);
    }

    auto operator<<(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value << rhs);
    }

    auto operator>>(TaggedFundamental const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value >> rhs.value);
    }

    auto operator>>(FundamentalType const &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value >> rhs);
    }

    ///////////////////////////////////////////////////
    /// Compound assignment bitwise operator overloads
    ///////////////////////////////////////////////////
    auto operator&=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value &= rhs.value;
        return *this;
    }

    auto operator&=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value &= rhs;
        return *this;
    }

    auto operator|=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value |= rhs.value;
        return *this;
    }

    auto operator|=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value |= rhs;
        return *this;
    }

    auto operator^=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value ^= rhs.value;
        return *this;
    }

    auto operator^=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value ^= rhs;
        return *this;
    }

    auto operator<<=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value <<= rhs.value;
        return *this;
    }

    auto operator<<=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
    {
        value <<= rhs;
        return *this;
    }

    auto operator>>=(TaggedFundamental const &rhs) noexcept -> TaggedFundamental &
    {
        value >>= rhs.value;
        return *this;
    }

    auto operator>>=(FundamentalType const &rhs) noexcept -> TaggedFundamental &
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
  private:
    std::string _file;
    uint32_t _line;
    std::string _func;

  public:
    SourceCodeLocation() = delete;

    /// @brief The only valid constructor
    ///
    /// @param `file` name of the file where the error occurred
    /// @param `line` line number where the error occurred
    /// @param `func` name of the function where the error occurred
    SourceCodeLocation(std::string_view const &file, uint32_t line, std::string_view const &func) noexcept
        : _file(file), _line(line), _func(func)
    {
    }

    /// @brief Default Destructor, Move/Copy constructor and assignment
    virtual ~SourceCodeLocation() = default;
    SourceCodeLocation(SourceCodeLocation &&other) noexcept = default;
    auto operator=(SourceCodeLocation &&other) noexcept -> SourceCodeLocation & = default;
    SourceCodeLocation(SourceCodeLocation const &other) = default;
    auto operator=(SourceCodeLocation const &other) -> SourceCodeLocation & = default;

  public:
    /// @brief Get the file name in which the error occured
    [[nodiscard]] inline auto file() const noexcept -> std::string_view
    {
        return _file;
    }

    /// @brief Get the line number in which the error occured
    [[nodiscard]] inline auto line() const noexcept -> uint32_t
    {
        return _line;
    }

    /// @brief Get the name of the function in which the error occured
    [[nodiscard]] inline auto function() const noexcept -> std::string_view
    {
        return _func;
    }
};

/// @brief Wrapper macro which constructs an instance of SourceCodeLocation in-place
/// and guarantees that the accurate file, function name, and line number will be
/// reported.
#ifdef __linux__
#define RUNTIME_INFO SourceCodeLocation(__FILE__, __LINE__, static_cast<const char *>(__PRETTY_FUNCTION__))
#else
#define RUNTIME_INFO SourceCodeLocation(__FILE__, __LINE__, static_cast<const char *>(__func__))
#endif

/// @brief Interface for an Error class
class IError
{
  public:
    IError() = default;
    virtual ~IError() = default;
    IError(IError &&other) noexcept = default;
    auto operator=(IError &&other) noexcept -> IError & = default;
    IError(IError const &other) = default;
    auto operator=(IError const &other) -> IError & = default;

  public:
    [[nodiscard]] virtual inline auto msg() const noexcept -> std::string = 0;
    [[nodiscard]] virtual inline auto info() const noexcept -> std::string = 0;
};

/// @brief A basic Error object which can be built using an error message and the
/// SourceCodeLocation RUNTIME_INFO macro.
class Error : public IError
{
  private:
    std::string _msg;
    std::string _info;

  private:
    /// @brief Constructs the error with only a message
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    explicit Error(std::string_view const &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    Error(std::string_view const &msg, SourceCodeLocation const &slc) noexcept : _msg(msg)
    {
        _info.append("Error: ")
            .append(msg)
            .append("\nFunction: ")
            .append(slc.function())
            .append("\nFile: ")
            .append(slc.file())
            .append(":")
            .append(std::to_string(slc.line()));
    }

  public:
    /// @brief Default Destructor, Move/Copy constructor and assignment
    ~Error() override = default;
    Error(Error &&other) noexcept = default;
    auto operator=(Error &&other) noexcept -> Error & = default;
    Error(Error const &other) = default;
    auto operator=(Error const &other) -> Error & = default;

  public:
    /// @brief Creates an Error object with only an error message via string_view
    [[nodiscard]] inline static auto create(std::string_view const &msg) -> Error
    {
        auto error = Error(msg);
        return error;
    }

    /// @brief Creates an Error object with error message and source location information
    /// using move semantics
    [[nodiscard]] inline static auto create(std::string_view const &msg, SourceCodeLocation const &slc) -> Error
    {
        auto error = Error(msg, slc);
        return error;
    }

  public:
    /// @brief Get just the error message
    [[nodiscard]] inline auto msg() const noexcept -> std::string override
    {
        return _msg;
    }

    /// @brief Override the current error message, useful when using the Result.mapErr method.
    [[nodiscard]] inline auto set(std::string_view const &msg) noexcept
    {
        _msg = msg;
    }

    /// @brief Get the pre-formatted (pretty printed) error string.
    ///
    /// @details  If Error was not created with the RUNTIME_INFO macro info_ will be empty,
    /// in which case the msg_ will be returned instead.
    [[nodiscard]] inline auto info() const noexcept -> std::string override
    {
        if (!_info.empty())
        {
            return _info;
        }
        return _msg;
    }
};

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
  private:
    std::variant<OkType, ErrType> _result;
    bool _is_ok{false};

  public:
    /// @brief All the constructors needed to build an OkType or ErrType
    Result() noexcept = default;
    explicit Result(OkType const &value) noexcept : _result(value), _is_ok(true)
    {
    }
    explicit Result(OkType &&value) noexcept : _result(std::move(value)), _is_ok(true)
    {
    }
    explicit Result(ErrType const &error) noexcept : _result(error)
    {
    }
    explicit Result(ErrType &&error) noexcept : _result(std::move(error))
    {
    }

    /// @brief Default Destructor, Move/Copy constructor and assignment
    virtual ~Result() = default;
    Result(Result &&other) noexcept = default;
    auto operator=(Result &&other) noexcept -> Result & = default;
    Result(const Result &other) = default;
    auto operator=(const Result &other) -> Result & = default;

  public:
    /// @brief Check if the variant is of the [OkType]
    [[nodiscard]] inline auto is_ok() const noexcept -> bool
    {
        return _is_ok;
    }

    /// @brief Check if the variant is of the [ErrType]
    [[nodiscard]] inline auto is_err() const noexcept -> bool
    {
        return !_is_ok;
    }

    /// @brief Get the OkType value
    ///
    /// @details The use should always use is_ok() before using ok()
    ///
    /// @return std::optinal<OkType> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] inline auto ok() const noexcept -> std::optional<OkType>
    {
        if (_is_ok)
        {
            if (auto *value = std::get_if<OkType>(&_result))
            {
                return *value;
            }
        }
        return std::nullopt;
    }

    /// @brief Get the ErrType value
    ///
    /// @details The use should always use is_err() before using err()
    ///
    /// @return std::optinal<ErrType> for safety, incase the user did not call
    /// is_err() before using this method.
    [[nodiscard]] inline auto err() const noexcept -> std::optional<ErrType>
    {
        if (!_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&_result))
            {
                return *err;
            }
        }
        return std::nullopt;
    }

    /// @brief Maps a custom/lambda function to the [OkType] leaving the [ErrType] untouched.
    ///
    /// @details The use should always use is_ok() before using map()
    ///
    /// @return Returns the Result type with a modified OkType, or just the Result unmodified
    /// if is_ok() is false.
    template <typename Function>
    [[nodiscard]] inline auto map(Function &&func) const noexcept -> Result<OkType, ErrType>
    {
        if (_is_ok)
        {
            if constexpr (std::is_invocable_r_v<OkType, Function, OkType const &>)
            {
                return Result<OkType, ErrType>(
                    std::invoke(std::forward<Function>(func), *std::get_if<OkType>(&_result)));
            }
            else
            {
                return Result<OkType, ErrType>(*std::get_if<OkType>(&_result));
            }
        }
        return Result<OkType, ErrType>(std::move(*std::get_if<ErrType>(&_result)));
    }

    /// @brief Maps a custom/lambda function to the [ErrType] leaving the [OkType] untouched.
    ///
    /// @details The use should always use the is_err() method before using mapErr().
    ///
    /// @return Returns the Result type with a modified ErrType, or just the Result unmodified
    /// if is_err() is false.
    template <typename Function>
    [[nodiscard]] inline auto map_err(Function &&func) const noexcept -> Result<OkType, ErrType>
    {
        if (!_is_ok)
        {
            if constexpr (std::is_invocable_r_v<ErrType, Function, ErrType const &>)
            {
                return Result<OkType, ErrType>(
                    std::invoke(std::forward<Function>(func), *std::get_if<ErrType>(&_result)));
            }
            else
            {
                return Result<OkType, ErrType>(*std::get_if<ErrType>(&_result));
            }
        }
        return Result<OkType, ErrType>(std::move(*std::get_if<OkType>(&_result)));
    }
};

/// @brief Result Template Specialization for std::unique_ptr.
///
/// @details Since std::unique_ptr is a move only type, the generic Result implementation
/// confuses which constructors are to be used and its not enough. Objects with deleted copy constructors
/// will need a templated specialization. You can use this specialiation for your custom type as
/// an example on how to do it.
template <typename OkType, typename ErrType> class Result<std::unique_ptr<OkType>, ErrType>
{
  private:
    std::variant<std::unique_ptr<OkType>, ErrType> _result;
    bool _is_ok{false};

  public:
    /// @brief All the constructors needed to build an OkType or ErrType for a move only type
    Result() noexcept = default;
    explicit Result(std::unique_ptr<OkType> &&value) noexcept : _result(std::move(value)), _is_ok(true)
    {
    }
    explicit Result(ErrType const &error) noexcept : _result(error)
    {
    }
    explicit Result(ErrType &&error) noexcept : _result(std::move(error))
    {
    }

  public:
    /// @brief Check if the variant value is of the [OkType]
    [[nodiscard]] inline auto is_ok() const noexcept -> bool
    {
        return _is_ok;
    }

    /// @brief Check if the variant value is of the [ErrType]
    [[nodiscard]] inline auto is_err() const noexcept -> bool
    {
        return !_is_ok;
    }

    /// @brief Gets the [OkType] value from the variant
    ///
    /// @details The user should always use the is_ok() method before using ok()
    ///
    /// @return std::optinal<OkType> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] inline auto ok() const noexcept -> std::optional<std::unique_ptr<OkType>>
    {
        if (_is_ok)
        {
            if (auto *value = std::get_if<std::unique_ptr<OkType>>(&_result))
            {
                return std::make_unique<OkType>(**value);
            }
        }
        return std::nullopt;
    }

    /// @brief Get the [ErrType] value from the variant
    ///
    /// @details The user should always use the is_err() method before using err()
    ///
    /// @return std::optinal<ErrType> for safety, incase the user did not call
    /// is_err() before using this method.
    [[nodiscard]] inline auto err() const noexcept -> std::optional<ErrType>
    {
        if (!_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&_result))
            {
                return *err;
            }
        }
        return std::nullopt;
    }
};

} // namespace etl

#endif // __cplusplus >= 201702l

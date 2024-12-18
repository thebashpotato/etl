#pragma once

#include <etl.hpp>
#include <utility>

namespace Example
{

/// @brief Move only object
///
/// @details This showcases how to make a Result<T, E> template specialization for a move only type.
class MoveOnlyType
{
  public:
    // Ctor/Dtor
    MoveOnlyType() noexcept = default;
    explicit MoveOnlyType(int value) noexcept : m_value(value)
    {
    }
    ~MoveOnlyType() = default;

    // Copy constructor and assignment operator delete
    MoveOnlyType(const MoveOnlyType &other) = delete;
    auto operator=(const MoveOnlyType &other) -> MoveOnlyType & = delete;

    // Move constructor and assignment operator
    MoveOnlyType(MoveOnlyType &&other) = default;
    auto operator=(MoveOnlyType &&other) -> MoveOnlyType & = default;

  public:
    // Public member functions
    [[nodiscard]] auto value() const noexcept -> int
    {
        return m_value;
    }

  private:
    // Private member variables
    int m_value{};
};

} // namespace Example

/// @brief Hi-jack the etl namespace to add a custom template specialization for example::MoveOnlyType
namespace etl
{

template <typename ErrType> class Result<Example::MoveOnlyType, ErrType>
{
  public:
    Result() noexcept = default;
    explicit Result(Example::MoveOnlyType &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : m_result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : m_result(std::move(error))
    {
    }

  public:
    /// @brief Check if the union value is of the ok type
    [[nodiscard]] auto is_ok() const noexcept -> bool
    {
        return m_is_ok;
    }

    /// @brief Check if the union value is of the error type
    [[nodiscard]] auto is_err() const noexcept -> bool
    {
        return !m_is_ok;
    }

    /// @brief Gets the [OkType] value from the variant
    ///
    /// @details The user should always use the is_ok() method before using ok()
    ///
    /// @return std::optional<Example::MoveOnlyType> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<Example::MoveOnlyType>
    {
        std::optional<Example::MoveOnlyType> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<Example::MoveOnlyType>(&m_result))
            {
                ret.emplace(std::move(*value));
            }
        }
        return ret;
    }

    /// @brief Get the [ErrType] value from the variant
    ///
    /// @details The user should always use the is_err() method before using err()
    ///
    /// @return std::optional<ErrType> for safety, incase the user did not call
    /// is_err() before using this method.
    [[nodiscard]] auto err() noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!m_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&m_result))
            {
                ret.emplace(std::move(*err));
            }
        }
        return ret;
    }

  private:
    std::variant<Example::MoveOnlyType, ErrType> m_result;
    bool m_is_ok{};
};
} // namespace etl

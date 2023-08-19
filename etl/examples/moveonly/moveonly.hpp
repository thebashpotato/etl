#pragma once

#include <etl.hpp>


namespace example {

    /// @brief Move only object
    ///
    /// @details This showcases how to make a Result<T, E> template specialization for a move only type.
    class MoveOnlyType {
    private:
        // Private member variables
        int _value{};

    public:
        // Ctor/Dtor
        MoveOnlyType() noexcept = default;
        explicit MoveOnlyType(int value) noexcept : _value(value) {}
        virtual ~MoveOnlyType() = default;

        // Copy constructor and assignment operator delete
        MoveOnlyType(MoveOnlyType const &other) = delete;
        auto operator=(MoveOnlyType const &other) -> MoveOnlyType & = delete;

        // Move constructor and assignment operator
        MoveOnlyType(MoveOnlyType &&other) = default;
        auto operator=(MoveOnlyType &&other) -> MoveOnlyType & = default;

    public:
        // Public member functions
        [[nodiscard]] auto getValue() const noexcept -> int {
            return _value;
        }
    };
}// namespace example


/// @brief Hi-jack the etl namespace to add a custom template specialization for example::MoveOnlyType
namespace etl {

    template<typename ErrType>
    class Result<example::MoveOnlyType, ErrType> {
    private:
        std::variant<example::MoveOnlyType, ErrType> _result;
        bool _isOk{};

    public:
        Result() noexcept = default;
        explicit Result(example::MoveOnlyType &&value) noexcept : _result(std::move(value)), _isOk(true) {}
        explicit Result(const ErrType &error) noexcept : _result(error) {}
        explicit Result(ErrType &&error) noexcept : _result(std::move(error)) {}

    public:
        /// @brief Check if the union value is of the ok type
        [[nodiscard]] inline auto isOk() const noexcept -> bool {
            return _isOk;
        }


        /// @brief Check if the union value is of the error type
        [[nodiscard]] inline auto isErr() const noexcept -> bool {
            return !_isOk;
        }


        /// @brief Check if the union value is of the error type
        ///
        /// @details The use should always use isOk() before using ok()
        ///
        /// @return std::optinal<example::MoveOnlyType> for safety, incase the user did not call
        /// isOk() before using this method.
        [[nodiscard]] inline auto ok() noexcept -> std::optional<example::MoveOnlyType> {
            std::optional<example::MoveOnlyType> opt{std::nullopt};
            if (_isOk) {
                if (auto *value = std::get_if<example::MoveOnlyType>(&_result)) {
                    opt.emplace(std::move(*value));
                    return opt;
                }
            }
            return opt;
        }


        /// @brief Check if the union value is of the error type
        ///
        /// @details The use should always use isErr() before using err()
        ///
        /// @return std::optinal<ErrType> for safety, incase the user did not call
        /// isErr() before using this method.
        [[nodiscard]] inline auto err() const noexcept -> std::optional<ErrType> {
            if (!_isOk) {
                if (auto *err = std::get_if<ErrType>(&_result)) {
                    return *err;
                }
            }
            return std::nullopt;
        }
    };
}// namespace etl
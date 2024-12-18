#include "moveonly.hpp"
#include <etl.hpp>
#include <iostream>
#include <utility>

using namespace etl;
using namespace Example;

/// @brief Build a move only type from an integer, to show that we can return a move only type from a function
/// inside a Result<T, E> template specialization.
auto BuildMoveOnlyType(int val) -> Result<MoveOnlyType, BaseError>
{
    if (val < 0)
    {
        return Result<MoveOnlyType, BaseError>({"Value must be positive", RUNTIME_INFO});
    }
    return Result<MoveOnlyType, BaseError>(MoveOnlyType(val));
}

auto main() -> int
{
    constexpr auto val = 42;
    if (auto result = BuildMoveOnlyType(val); result.is_ok())
    {
        const MoveOnlyType moveOnlyType = std::move(*result.ok());
        std::cout << "Result is ok" << '\n';
        std::cout << "Value is: " << moveOnlyType.value() << '\n';
    }
    else
    {
        const BaseError error = std::move(*result.err());
        std::cout << "Result is not ok" << '\n';
        std::cout << error.info() << '\n';
    }

    return 0;
}

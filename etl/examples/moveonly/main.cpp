#include "moveonly.hpp"
#include <iostream>

using namespace etl;
using namespace example;

/// @brief Build a move only type from an integer, to show that we can return a move only type from a function
/// inside a Result<T, E> template specialization.
auto buildMoveOnlyType(int val) -> Result<MoveOnlyType, std::string> {
    if (val < 0) {
        return Result<MoveOnlyType, std::string>(std::string("Value must be positive"));
    }
    return Result<MoveOnlyType, std::string>(MoveOnlyType(val));
}

auto main() -> int {
    constexpr auto val = 42;
    auto result = buildMoveOnlyType(val);
    if (result.isOk()) {
        auto moveOnlyType = std::move(result.ok().value());
        std::cout << "Result is ok" << std::endl;
        std::cout << "Value is " << moveOnlyType.getValue() << std::endl;
    } else {
        std::cout << "Result is not ok" << std::endl;
        std::cout << "Error is " << result.err().value() << std::endl;
    }
    return 0;
}
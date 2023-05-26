#include <cstdlib>
#include <etl.hpp>
#include <iostream>

using namespace etl;

auto divide(int numerator, int denominator) noexcept -> Result<int, std::string> {
    if (denominator == 0) {
        return Result<int, std::string>("Division by zero");
    }
    return Result<int, std::string>(numerator / denominator);
}

/**
 * @brief A scratch file for interactively testing code
 * */
auto main() -> int {
    constexpr auto numerator = 10;
    constexpr auto denominator = 0;

    auto result = divide(numerator, denominator);
    std::cout << result.err().value() << '\n';

    Result<int, std::string> appendResult = result.mapErr([](std::string &msg) -> std::string {
        return "Error: " + msg;
    });

    std::cout << appendResult.err().value() << '\n';


    return EXIT_SUCCESS;
}

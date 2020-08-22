#include "calculator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>


int main() {
    std::string input;
    getline(std::cin, input);
    std::replace(input.begin(), input.end(), ',', '.');

    try {
        auto result = calculate(input);
        std::cout << std::setprecision(2) << result << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "некорректный ввод, строка содержит недопустимое выражение " << e.what() << '\n';
    }
}

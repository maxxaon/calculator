#include "calculator.h"

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>


void testIsOperation() {
    std::vector<std::pair<char, bool>> in_out =
            {{'+', true}, {'*', true}, {'(', false}, {'a', false}};
    for (auto [in, out] : in_out) {
        assert(isOperation(in) == out);
    }
};

void testGetPriority() {
    std::vector<char> operations = {'+', '*', '(', '-', '/', ')'};
    std::sort(operations.begin(), operations.end(), [] (char lhs, char rhs) {
        auto lhs_priority = getPriority(lhs);
        auto rhs_priority = getPriority(rhs);
        return lhs_priority != rhs_priority ? lhs_priority < rhs_priority : lhs < rhs;
    });
    std::vector<char> expected = {'(', ')', '+', '-', '*', '/'};
    assert(operations == expected);
}

void assertEqualWithEps(double lhs, double rhs) {
    static const double eps = 1e-8;
    assert(abs(lhs - rhs) < eps);
}

void testGetOperationResult() {
    assertEqualWithEps(getOperationResult(1, 2, '-'), -1);
    assertEqualWithEps(getOperationResult(2, -3, '*'), -6);
    assertEqualWithEps(getOperationResult(6, 2, '/'), 3);
    assertEqualWithEps(getOperationResult(1, 2, '+'), 3);
}

void testMakeOperation() {
    {
        std::stack<double> numbers({2, 3});
        makeOperation(numbers, '*');
        assert(numbers.size() == 1);
        assertEqualWithEps(numbers.top(), 6);
    }
    {
        std::stack<double> numbers({4});
        makeOperation(numbers, -'-');
        assert(numbers.size() == 1);
        assertEqualWithEps(numbers.top(), -4);
    }
}

void testProcessOperation() {
    std::stack<double> numbers({1, 24, 3, 4});
    std::stack<char> symbols({'+', '/', '*'});
    processOperation(numbers, symbols, '*');

    std::stack<double> expected_numbers({1, 2});
    std::stack<char> expected_symbols({'+', '*'});
    assert(numbers == expected_numbers && symbols == expected_symbols);
}

void testProcessCloseBracket() {
    std::stack<double> numbers({1, 2, 3, 6});
    std::stack<char> symbols({'+', '(', '*', '-'});
    processCloseBracket(numbers, symbols);

    std::stack<double> expected_numbers({1, -6});
    std::stack<char> expected_symbols({'+'});
    assert(numbers == expected_numbers && symbols == expected_symbols);
}

void testCalculate() {
    assertEqualWithEps(calculate("-1 + 5 - 3"), 1);
    assertEqualWithEps(calculate("-10 + (8 * 2.5) - (3 / 1.5)"), 8);
    assertEqualWithEps(calculate("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)"), 11);
}

int main() {
    testIsOperation();
    testGetPriority();
    testGetOperationResult();
    testMakeOperation();
    testProcessOperation();
    testProcessCloseBracket();
    testCalculate();

    std::cout << "OK" << std::endl;
}

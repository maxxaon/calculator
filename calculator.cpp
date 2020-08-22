#include "calculator.h"

#include <sstream>
#include <stdexcept>


bool isOperation(char c) {
    static std::string operations = "-+*/";
    return operations.find(c) != std::string::npos;
}

int getPriority(char operation) {
    if (operation < 0) {
        return 3;
    }
    return operation == '+' || operation == '-' ? 1 :
           operation == '*' || operation == '/' ? 2 :
           -1;
}

double getOperationResult(double lhs, double rhs, char operation) {
    switch (operation) {
        case '+':  {
            return lhs + rhs;
        }
        case '-':  {
            return lhs - rhs;
        }
        case '*': {
            return lhs * rhs;
        }
        case '/':  {
            return lhs / rhs;
        }
        default: {
            throw std::logic_error("unknown operation " + std::string(operation, 1));
        }
    }
}

void makeOperation(std::stack<double>& numbers, char operation) {
    auto rhs = numbers.top();
    numbers.pop();
    double lhs = 0;
    if (operation > 0) {
        lhs = numbers.top();
        numbers.pop();
    } else {
        operation *= -1;
    }

    numbers.push(getOperationResult(lhs, rhs, operation));
}

void processOperation(
        std::stack<double>& numbers, std::stack<char>& symbols, char operation) {
    while (!symbols.empty()
           && getPriority(symbols.top()) >= getPriority(operation)) {
        makeOperation(numbers, symbols.top()), symbols.top();
        symbols.pop();
    }
    symbols.push(operation);
}

void processCloseBracket(std::stack<double>& numbers, std::stack<char>& symbols) {
    while (symbols.top() != '(') {
        makeOperation(numbers, symbols.top());
        symbols.pop();
    }
    symbols.pop();
}

double calculate(const std::string& expression) {
    std::istringstream input_stream(expression);
    std::stack<double> numbers;
    std::stack<char> symbols;
    char c;
    bool is_next_operator_unary = true;
    while ((c = input_stream.peek()) != EOF) {
        if (isdigit(c)) {
            double number;
            input_stream >> number;
            numbers.push(number);
            is_next_operator_unary = false;
        } else {
            if (c == '(') {
                symbols.push(c);
                is_next_operator_unary = true;
            } else if (c == ')') {
                processCloseBracket(numbers, symbols);
                is_next_operator_unary = false;
            } else if (isOperation(c)) {
                if (is_next_operator_unary) {
                    c *= -1;
                }
                processOperation(numbers, symbols, c);
                is_next_operator_unary = true;
            } else if (isspace(c)) {

            } else {
                std::string bad_expression;
                while (c != EOF && !isspace(c) && !isdigit(c) && !isOperation(c) && c != '(' && c != ')') {
                    bad_expression.push_back(c);
                    input_stream.ignore(1);
                    c = input_stream.peek();
                }
                throw std::invalid_argument(bad_expression);
            }
            input_stream.ignore(1);
        }
    }
    while (!symbols.empty())
        makeOperation(numbers, symbols.top()),  symbols.pop();
    return numbers.top();
}

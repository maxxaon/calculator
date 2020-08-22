#pragma once

#include <stack>
#include <string>


bool isOperation(char c);

int getPriority(char operation);

double getOperationResult(double lhs, double rhs, char operation);

void makeOperation(std::stack<double>& numbers, char operation);

void processOperation(
        std::stack<double>& numbers, std::stack<char>& symbols, char operation);

void processCloseBracket(std::stack<double>& numbers, std::stack<char>& symbols);

double calculate(const std::string& expression);

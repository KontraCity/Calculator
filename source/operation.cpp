#include "operation.hpp"

bool IsOperation(char character)
{
    if (character == '+' || character == '-')
        return true;
    if (character == '*' || character == '/')
        return true;
    if (character == '(' || character == ')')
        return true;
    if (character == '^')
        return true;
    return false;
}

int ToPrecedence(Operation operation)
{
    if (operation == '+' || operation == '-')
        return 2;
    if (operation == '*' || operation == '/')
        return 3;
    if (operation == '^')
        return 4;
    return -1;
}

double Calculate(double left, Operation operation, double right)
{
    switch (operation)
    {
        default:
            return 0.0;
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            if (right == 0.0)
                throw runtime_error("Can't divide by zero");
            return left / right;
        case '^':
            return pow(left, right);
    }
}

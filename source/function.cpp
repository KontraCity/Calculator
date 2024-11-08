#include "function.hpp"

Function ToFunction(const string& string)
{
    if (string == "sin")
        return Function::Sinus;
    if (string == "cos")
        return Function::Cosinus;
    if (string == "tan")
        return Function::Tangent;
    if (string == "cot")
        return Function::Cotangent;
    return Function::None;
}

const char* ToString(Function function)
{
    switch (function)
    {
        default:
            return "none";
        case Function::Sinus:
            return "sin";
        case Function::Cosinus:
            return "cos";
        case Function::Tangent:
            return "tan";
        case Function::Cotangent:
            return "cot";
    }
}

double Calculate(double value, Function function)
{
    switch (function)
    {
        default:
            return 0.0;
        case Function::Sinus:
            return sin(value);
        case Function::Cosinus:
            return cos(value);
        case Function::Tangent:
            return sin(value) / cos(value);
        case Function::Cotangent:
            if (sin(value) == 0.0)
                throw runtime_error("Can't divide by zero");
            return cos(value) / sin(value);
    }
}

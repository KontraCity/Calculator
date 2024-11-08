#pragma once

// STL modules
#include <string>
#include <stdexcept>
using namespace std;

enum class Function
{
    None,
    Sinus,
    Cosinus,
    Tangent,
    Cotangent,
};

Function ToFunction(const string& string);

const char* ToString(Function function);

double Calculate(double value, Function function);

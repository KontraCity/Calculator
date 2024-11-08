#pragma once

// STL modules
#include <stdexcept>
using namespace std;

using Operation = char;

bool IsOperation(char character);

int ToPrecedence(Operation operation);

double Calculate(double left, Operation operation, double right);

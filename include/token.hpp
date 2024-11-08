#pragma once

// STL modules
#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

// Custom modules
#include "function.hpp"
#include "operation.hpp"

class Token : public variant<double, Operation, Function>
{
public:
    enum Type
    {
        Number,
        Operation,
        Function,
    };

public:
    using variant::variant;

    inline Type type() const
    {
        return static_cast<Type>(index());
    }
};

void AddToken(vector<Token>& tokens, std::string& token);

vector<Token> Tokenize(string input);

void Process(vector<Token>& tokens);

void Print(const std::string& comment, const vector<Token>& tokens);

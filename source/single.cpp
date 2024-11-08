// STL modules
#include <iostream>
#include <iomanip>
#include <variant>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

//////////////////// Operations ////////////////////
using Operation = char;

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

//////////////////// Functions ////////////////////
enum class Function
{
    None,
    Sinus,
    Cosinus,
    Tangent,
    Cotangent,
};

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

//////////////////// Tokens ////////////////////
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

void AddToken(vector<Token>& tokens, std::string& token)
{
    if (token.empty())
        return;

    Function function = ToFunction(token);
    if (function != Function::None)
    {
        tokens.push_back(function);
        token.clear();
        return;
    }

    try
    {
        if (token == "PI")
            tokens.push_back(3.14159265358979323846);
        else if (token == "E")
            tokens.push_back(2.71828182845904523536);
        else
            tokens.push_back(stod(token));
        token.clear();
    }
    catch (...)
    {
        throw runtime_error("Unknown token: \"" + token + '\"');
    }
}

vector<Token> Tokenize(string input)
{
    // Tokenize input string
    vector<Token> tokens;
    string token;
    for (char character : input)
    {
        if (character == ' ')
            continue;

        if (IsOperation(character))
        {
            AddToken(tokens, token);
            tokens.push_back(character);
            continue;
        }
        token += character;
    }
    AddToken(tokens, token);

    // Sort tokens
    vector<Token> result, operations;
    for (const Token& token : tokens)
    {
        if (token.type() == Token::Number)
        {
            result.push_back(token);
            continue;
        }

        if (token.type() == Token::Function || get<Operation>(token) == '(')
        {
            operations.push_back(token);
            continue;
        }

        if (get<Operation>(token) == ')')
        {
            while (!operations.empty() && get<Operation>(operations.back()) != '(')
            {
                result.push_back(operations.back());
                operations.pop_back();
            }
            operations.pop_back();

            if (!operations.empty() && operations.back().type() == Token::Function)
            {
                result.push_back(operations.back());
                operations.pop_back();
            }
            continue;
        }

        while (!operations.empty() && get<Operation>(operations.back()) != '(' && ToPrecedence(get<Operation>(operations.back())) >= ToPrecedence(get<Operation>(token)))
        {
            result.push_back(operations.back());
            operations.pop_back();
        }
        operations.push_back(token);
    }

    while (!operations.empty())
    {
        result.push_back(operations.back());
        operations.pop_back();
    }
    return result;
}

void Process(vector<Token>& tokens)
{
    while (true)
    {
        auto entry = std::find_if(tokens.begin(), tokens.end(), [](const Token& token) { return token.type() != Token::Number; });
        if (entry == tokens.end())
            break;

        double result = 0.0;
        int operands = entry->type() == Token::Operation ? 2 : 1;
        if (operands + 1 > tokens.size())
            throw runtime_error("Invalid formula");

        if (operands == 2)
        {
            double left = get<double>(*(entry - 2));
            double right = get<double>(*(entry - 1));
            Operation operation = get<Operation>(*entry);
            result = Calculate(left, operation, right);
        }
        else
        {
            double value = get<double>(*(entry - 1));
            Function function = get<Function>(*entry);
            result = Calculate(value, function);
        }

        size_t entryIndex = entry - tokens.begin();
        tokens.erase(entry - operands, entry + 1);
        tokens.empty() ? tokens.push_back(result) : tokens.insert(tokens.begin() + (entryIndex - operands), result);
    }
}

void Print(const std::string& comment, const vector<Token>& tokens)
{
    cout << comment;
    for (const Token& token : tokens)
    {
        if (token.type() == Token::Number)
            cout << get<double>(token) << ' ';
        else if (token.type() == Token::Operation)
            cout << get<Operation>(token) << ' ';
        else
            cout << ToString(get<Function>(token)) << ' ';
    }
    cout << '\n';
}

//////////////////// main() ////////////////////
int main()
{
    cout << "Input:\t\t";
    string input;
    getline(cin, input);

    try
    {
        if (input.find('x') != string::npos)
        {
            string checkInput = input;
            replace(checkInput.begin(), checkInput.end(), 'x', '1');
            Process(Tokenize(checkInput));

            cout << "Enter X:\t";
            double x;
            cin >> x;

            while (true)
            {
                size_t location = input.find('x');
                if (location == string::npos)
                    break;
                input.replace(location, 1, to_string(x));
            }
        }

        vector<Token> tokens = Tokenize(input);
        Print("RPN Formula:\t", tokens);
        Process(tokens);
        cout << "Result:\t\t" << fixed << get<double>(tokens[0]) << '\n';
        return 0;
    }
    catch (const exception& error)
    {
        cout << "Error: " << error.what() << '\n';
        cout << "+-------------------------------------------------+\n";
        cout << "|                     Help:                       |\n";
        cout << "| Available functions: sin(), cos(), tan(), cot() |\n";
        cout << "| Available variables: x                          |\n";
        cout << "| Available constants: PI, E                      |\n";
        cout << "+-------------------------------------------------+\n";
        return 1;
    }
}

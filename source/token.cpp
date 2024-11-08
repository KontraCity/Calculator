#include "token.hpp"

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

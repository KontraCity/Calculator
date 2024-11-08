// STL modules
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

// Custom modules
#include "token.hpp"

static void ShowHelp()
{
    cout << "+-------------------------------------------------+\n";
    cout << "|                     Help:                       |\n";
    cout << "| Available functions: sin(), cos(), tan(), cot() |\n";
    cout << "| Available variables: x                          |\n";
    cout << "| Available constants: PI, E                      |\n";
    cout << "+-------------------------------------------------+\n";
}

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
        ShowHelp();
        return 1;
    }
}

// to use data types
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <cstring>
#include <fstream>


// using namespace std;

// GOAL of file: define the tokens that our language will support

// this tells us the distinct constant type 
enum TokenType
{
    Number,
    Identifier,
    Equals,
    CurlyOpenParen,
    CurlyCloseParen,
    OpenParen,
    CloseParen,
    BinaryOperator,
    Let,
    If,
    Print
};

// groups together variables under Token name
struct Token
{
    std::string value;
    TokenType type;

    // static are shared among all instances of a class or struct
    // dereference for better memory storage, pass by reference
    // &val has to be constant because you are binding to a non constant
    // reference 
    static Token token(const std::string& val, TokenType typ) 
    {
        return Token{val, typ};
    }
};

// difference between .front() and .begin()
// .front() = get a reference to the first element in the container
// .begin() = get iterator to the first element in the ocntainer
std::string shift(std::vector<std::string> &src) 
{
    std::string curr = src.front();
    // this erases the reference to the first element and shifts everything over
    src.erase(src.begin());
    return curr;
}

// checking if it is a character in the alphabet
bool isAlpha(const std::string &str)
{
    for (char ch: str)
    {
        if (!isalpha(ch))
        {
            return false;
        }
    }
    return true;
}

bool isNumber(const std::string &str)
{
    for (char ch: str)
    {
        if (!isdigit(ch)){
            return false;
        }
    }
    return true;
}

bool isSingleComment(const std::string &str)
{
    std::cout << "comment function" << std::endl;
    return str.rfind("comment:", 0) == 0;
}

// checking if we can actually skip the character
// not taking in '/n' to be able to know when we are hadling a new line for commenting
bool isSkippable(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

// Find if it is a recognizable identifier
typedef std::map<std::string, TokenType> ReservedIdentifierMap;
ReservedIdentifierMap reservedIdentifier;

void INIT_RESERVED_IDENTIFIER()
{
    reservedIdentifier["let"] = TokenType::Let;
    reservedIdentifier["if"] = TokenType::If;
    reservedIdentifier["p"] = TokenType::Print;
}

std::vector<std::string> splitString(const std::string &sourceCode){
    std::vector<std::string> words;
    std::string word;
    bool comment = false;

    for (size_t i = 0; i < sourceCode.size(); ++i)
    {
        char c = sourceCode[i];
        // std::cout << c << std::endl;
        if (comment)
        {
            if (c == '\n')
            {
                // std::cout << "turn comment off" << std::endl;
                comment = false;
            }
            continue;
        }

        if (isSkippable(c))
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '+' || c == '-' || c == '/' || c == '*')
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
            word += c;
            words.push_back(word);
            word.clear();
        }
        else{
            word += c;
            if (word == "comment:")
            {
                comment = true;
                word.clear();
            }
        }
    }
    if (!word.empty() && !comment)
    {
        words.push_back(word);
        word.clear();
    }
    return words;
}


// function takes source code as input and returns a vector containing tokens
std::vector<Token> tokenize(const std::string &sourceCode) 
{
    // variable to store the tokens
    std::vector<Token> tokens;

    // variable that splits each string with a space in a vector
    std::vector<std::string> src = splitString(sourceCode);

    // START WITH IMPLEMENTING SHIFT
    while (!src.empty()) 
    {
        // std::cout << "Current token: " << src.front() << std::endl;

        if (src.front() == "(") 
        {
            // shift is executed before the push_back
            tokens.push_back(Token::token(shift(src), TokenType::OpenParen));

        }
        else if (src.front() == ")")
        {
            tokens.push_back(Token::token(shift(src), TokenType::CloseParen));

        }
        else if (src.front() == "{")
        {
            tokens.push_back(Token::token(shift(src), TokenType::CurlyOpenParen));

        }
        else if (src.front() == "}")
        {
            tokens.push_back(Token::token(shift(src), TokenType::CurlyCloseParen));

        }
        else if (src.front() == "=")
        {
            tokens.push_back(Token::token(shift(src), TokenType::Equals));

        }
        else if (src.front() == "+" || src.front() == "-" || src.front() == "*" || src.front() == "/")
        {
            tokens.push_back(Token::token(shift(src), TokenType::BinaryOperator));

        }
        else
        {
            // multicharacter token
            if (isNumber(src.front()))
            {
                // variable to store multiple digit number
                std::string number;

                while (!src.empty() && isNumber(src.front()))
                {
                    number += shift(src);
                }

                tokens.push_back(Token::token(number, TokenType::Number));
            }
            else if (isAlpha(src.front()))
            {
                // like for let word = 10, we are focusing on the let
                std::string identifier = shift(src);

                // implement ReservedIdentMap later
                ReservedIdentifierMap::iterator it = reservedIdentifier.find(identifier);
                // the .end() points to a non-existant end iterator 
                if (it != reservedIdentifier.end())
                {
                    tokens.push_back(Token::token(identifier, it->second));
                }
                else
                {
                    tokens.push_back(Token::token(identifier, TokenType::Identifier));
                }
            }
            // using first character
            else if (isSkippable(src.front()[0]))
            {
                shift(src);
            }
            else
            {
                std::cout << "Unrecognized character found! " << src.front() << std::endl;
                exit(1);
            }

        }
    }
    return tokens;
}

// STILL DON'T REALLY KNOW WHAT ARGC AND ARGV IS FOR
// argc = number of commandline arguments passed to the program, always atleast 1
// argv = array of character arrays: argv[0] is the name of the program
int main(int argc, char *argv[])
{
    // check if the arguments are correct
    if (argc != 2)
    {
        // cerr is error stream
        std::cerr << "Incorrect Arguments" << std::endl;
        std::cerr << "Correct usage: ./dejavu <input file path --> input.vu>" << std::endl;
    }

    // checking if the input file is valid -> .vu ?
    // this block is called a compound statement or scope blocks
    {
        const char *ext = ".vu";
        size_t xlen = strlen(ext);
        size_t slen = strlen(argv[1]);
        // making sure the ext is the same
        int found = strcmp(argv[1] + slen - xlen, ext) == 0;
        if (found == 0)
        {
            std::cerr << "Invalid code file" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // initialize the variable
    INIT_RESERVED_IDENTIFIER();

    // read input source code file and convert it into a string stream

    // end goal is to put the code into sourceCode as a string
    std::string sourceCode;
    {
        // allows insertion, deletion, and such
        std::stringstream contents_stream;
        // it can write, read, and such from files
        // std::ios::in means it should be open in input mode
        std::fstream input(argv[1], std::ios::in);
        // get pointer to the buffer
        contents_stream << input.rdbuf();
        sourceCode = contents_stream.str();
    }

    std::vector<Token> tokens = tokenize(sourceCode);
    // we are using pre increment for i because the first value might not be important because it can be 
    // something like a placeholder for start.
    // std::cout << tokens.size() << std::endl;
    for (int i = 0; i < tokens.size(); ++i)
    {
        std::cout << "Value: " << tokens[i].value << " Type: " << tokens[i].type << std::endl;
    }

}

// Tommorow:
// figure out how to implement a parser now, AVL tree?

/* Current Progress:
    - able to define variables
    - if statement
    - print variables
    - use comments
*/



// I want to make my programming language:
// - not have any identifier like let or string or whatever like Python, this is great practice to be able to implement tests and such
// - be able to make comments by doing "Comment: "
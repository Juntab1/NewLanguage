// to use data types
#include <iostream>
#include <vector>
#include <sstream>

// using namespace std;

// GOAL of file: define the tokens that our language will support

// this tells us the distinct constant type 
enum TokenType
{
    Number,
    Identifier,
    Equals,
    OpenParen,
    CloseParen,
    BinaryOperator,
    Let,
};

// groups together variables under Token name
struct Token
{
    std::string value;
    TokenType type;

    // static are shared among all instances of a class or struct
    // dereference for better memory storage, pass by reference
    static Token token(std::string& val, TokenType typ){
        return Token{val, typ};
    }
};

// function takes source code as input and returns a vector containing tokens
std::vector<Token> tokenize(std::string &sourceCode) {
    // variable to store the tokens
    std::vector<Token> tokens;

    // store temp word to put into src 
    std::string currWord;

    // making stream from the string
    std::stringstream ss(sourceCode);

    // variable that splits each string with a space in a vector
    std::vector<std::string> src;

    // this while loop takes every word split with space from ss 
    // and puts the value into src
    while (getline(ss, currWord, ' ')){
        src.push_back(currWord);
        currWord.clear();
    }

    // START WITH IMPLEMENTING SHIFT
    while (!src.empty()) {
        if (src.front() == "(") {
            tokens.push_back(Token::token(shift(src), TokenType::OpenParen));

        }
    }
    return tokens;
}
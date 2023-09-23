#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>

enum State {
    START,
    INTEGER,
    IDENTIFIER
};

struct Token {
    std::string type;
    std::string value;
};

class Lexer {
private:
    State state = START;
    std::ifstream file;

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

public:
    Lexer(const std::string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file" << std::endl;
            exit(1);
        }
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string lexeme;
        char c;

        while (file.get(c)) {
            switch (state) {
                case START:
                    lexeme = "";
                    if (is_digit(c)) {
                        state = INTEGER;
                        lexeme += c;
                    } else if (is_alpha(c) || c == '_') {
                        state = IDENTIFIER;
                        lexeme += c;
                    }
                    break;

                case INTEGER:
                    if (is_digit(c)) {
                        lexeme += c;
                    } else {
                        tokens.push_back({"INTEGER", lexeme});
                        state = START;
                        file.unget(); // Return the character to the stream.
                    }
                    break;

                case IDENTIFIER:
                    if (is_alpha(c) || c == '_' || is_digit(c)) {
                        lexeme += c;
                    } else {
                        tokens.push_back({"IDENTIFIER", lexeme});
                        state = START;
                        file.unget(); // Return the character to the stream.
                    }
                    break;
            }
        }

        // Handle end of file.
        if (state == INTEGER) {
            tokens.push_back({"INTEGER", lexeme});
        } else if (state == IDENTIFIER) {
            tokens.push_back({"IDENTIFIER", lexeme});
        }

        return tokens;
    }
};

int main() {
    // Use the lexer to process the file "input_sourcecode.txt".
    Lexer lexer("input_sourcecode.txt");
    std::vector<Token> tokens = lexer.tokenize();

    // Print the identified tokens.
    for (const auto& token : tokens) {
        std::cout << token.type << " : " << token.value << std::endl;
    }

    return 0;
}
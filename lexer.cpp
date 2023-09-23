#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Lexer {
private:
    std::string source_code;
    std::string::iterator cursor;
    std::ofstream output_file;

    bool is_whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    std::string read_number() {
        std::string number;
        while (is_digit(*cursor)) {
            number += *cursor;
            cursor++;
        }
        return number;
    }

public:
    Lexer(const std::string& filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        source_code = buffer.str();
        cursor = source_code.begin();
        output_file.open("output.txt");
    }

    std::pair<std::string, std::string> lexer() {
        while (cursor != source_code.end()) {
            if (is_whitespace(*cursor)) {
                cursor++;
                continue;
            } else if (is_digit(*cursor)) {
                std::string lexeme = read_number();
                output_file << "NUMBER\t" << lexeme << std::endl;
                return {"NUMBER", lexeme};
            }
            // Extend this part for other token types.
        }
        return {"", ""};
    }

    void process() {
        while (cursor != source_code.end()) {
            lexer();
        }
        output_file.close();
    }
};

int main() {
    Lexer lexer("input_sourcecode.txt");
    lexer.process();

    std::cout << "Lexical Analysis Completed. Results saved in output.txt.\n";

    return 0;
}

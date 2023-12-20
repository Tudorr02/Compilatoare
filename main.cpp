#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cctype>

enum TokenType {
    IDENTIFIER, // var name, method name ,
    KEYWORD,// if , else, for , int
    SYMBOL, // + , - , ( ,....
    NUMBER, // 10
    FLOAT_NUMBER,
    STRING, // "smth"
    END,    //
    ERROR   //
};

std::vector <std::string> typeStrings = {
        "IDENTIFIER",
        "KEYWORD   ",
        "SYMBOL    ",
        "NUMBER    ",
        "FLOAT NUMBER",
        "STRING    ",
        "END       ",
        "ERROR     "
};

struct Token {
    TokenType type;
    std::string value;
};

enum State {
    START,
    IN_IDENTIFIER,
    IN_NUMBER,
    IN_FLOAT,
    IN_SYMBOL,
    IN_STRING,
    END_STRING
};


bool isKeyword(const std::string& word) {
    static const std::vector<std::string> keywords = {
            "typedef", "extern", "unsigned", "switch", "void",
            "const", "char", "continue", "volatile", "if",
            "auto", "for", "case", "float", "default",
            "do", "goto", "inline", "return", "break",
            "register", "int", "long", "signed", "sizeof",
            "static", "struct", "union", "while", "enum",
            "else", "restrict", "double"
    };

    for (const std::string& keyword : keywords) {
        if (word == keyword) {
            return true;
        }
    }
    return false;
}

unsigned int cursor = 0;
std::string input;

Token get_next_token() {
    State current_state = START;
    std::string collected_chars;
    for (;cursor < input.size(); cursor++) {
        char c = input[cursor];
        switch (current_state) {
            case START:
                if (isalpha(c)  || c == '_') {
                    collected_chars += c;
                    current_state = IN_IDENTIFIER;
                } else if (isdigit(c)) {
                    collected_chars += c;
                    current_state = IN_NUMBER;
                } else if(c == '"') {
                    collected_chars += c;
                    current_state = IN_STRING;
                } else if (!isspace(c)) {
                    collected_chars += c;
                    current_state = IN_SYMBOL;
                }
                break;

            case IN_IDENTIFIER:
                if (isalnum(c) || c == '_') {
                    collected_chars += c;
                }else if(c=='.' && std::isdigit(collected_chars[cursor-1])){
                    collected_chars += c;
                    std::string str;
                    for(int i=0;i<collected_chars.size();i++)
                        if(collected_chars[i])
                }
                else return {isKeyword(collected_chars) ? KEYWORD : IDENTIFIER, collected_chars};
                break;

            case IN_NUMBER: // to add float case 354.
                if (isdigit(c)) {
                    collected_chars += c;
                }else if(c == '.'){
                    collected_chars+=c;
                    current_state = IN_FLOAT ;
                }

                else return (Token){NUMBER, collected_chars};
                break;

            case IN_FLOAT:
                if (isdigit(c))
                    collected_chars += c;
                else return (Token){FLOAT_NUMBER, collected_chars};
                break;

            case IN_SYMBOL:
                return (Token){SYMBOL, collected_chars};

            case IN_STRING:
                collected_chars += c;
                if(c == '"') {
                    current_state = END_STRING;
                }
                break;

            case END_STRING:
                return (Token){STRING, collected_chars};
        }
    }
    if(collected_chars.empty()){
        return (Token){END, collected_chars};
    }
    else{ //abcd
        switch (current_state) {
            case IN_IDENTIFIER:
                return (Token){isKeyword(collected_chars) ? KEYWORD : IDENTIFIER, collected_chars};
            case IN_NUMBER:
                return (Token){NUMBER, collected_chars};
            case END_STRING:
                return (Token){STRING, collected_chars};
            case IN_SYMBOL:
                return (Token){SYMBOL, collected_chars};
            default:
                return (Token){ERROR, collected_chars};
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::ifstream inputFile(inputFilePath);


    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    input = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    Token tok;
    while(true){
        tok = get_next_token();
        if(tok.type == END)
            break;
        std::cout << typeStrings[tok.type] << " " << tok.value << std::endl;
    }


    return 0;
}



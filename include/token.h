#ifndef TOKEN_H
#define TOKEN_H

// Token type enumeration
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_NEWLINE,
    TOKEN_COMMENT,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;


// Token structure
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

#endif // TOKEN_H

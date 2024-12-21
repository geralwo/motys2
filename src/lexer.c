#include "lexer.h"
#include "motys_stdlib.h"
#include "token.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strndup(const char *str, int n)
{
    char *result = (char *)malloc(n + 1); // Allocate memory for the substring + null terminator
    if (result) {
        memcpy(result, str, n);        // Copy 'n' characters from the source string
        result[n] = '\0'; // Null-terminate the string
    }
    return result;
}

bool is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '$' || c == '@' || c == ':';
}

bool is_delimiter(char c)
{
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '<' || c == '>' || c == ',' || c == ';' || c == '.' || c == ':' || c == '=';
}

void lexer_init(Lexer *lexer, char *_source) {
    lexer->source = _source;
    lexer->length = strlen(_source);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->Tokens = motys_array_create(sizeof(Token), 32);
    printf("Lexer initialized\n");
}

void lexer_free(Lexer *lexer)
{
	// free(lexer->source); // double free?
	motys_array_free(lexer->Tokens);
}

void lexer_tokenize(Lexer *lexer) {
    // Create a dynamic array for tokens
    while (lexer->source[lexer->position] != '\0') {
        char c = lexer->source[lexer->position];

        // Handle whitespace
        if (isspace(c)) {
            if (c == '\n') {
                Token t = {TOKEN_NEWLINE, "NewLine", lexer->line,
                           lexer->column};
                motys_array_add(lexer->Tokens, &t);
                lexer->line++;
                lexer->column = 0;
            }
            lexer->position++;
            lexer->column++;
        }
        // Handle identifiers (letters or alphanumeric sequences starting
        // with a letter)
        else if (isalpha(c)) {
            int start = lexer->position;
            while (isalnum(lexer->source[lexer->position])) {
                lexer->position++;
                lexer->column++;
            }
            int length = lexer->position - start;
            char *value = strndup(&lexer->source[start], length);

            Token t = {TOKEN_IDENTIFIER, value, lexer->line, lexer->column};
            motys_array_add(lexer->Tokens, &t);
        } else if (isdigit(c)) {
            int start = lexer->position;
            while (isdigit(lexer->source[lexer->position]) ||
                   lexer->source[lexer->position] == '.') {
                lexer->position++;
                lexer->column++;
            }
            int length = lexer->position - start;
            char *value = strndup(&lexer->source[start], length);

            Token t = {TOKEN_NUMBER, value, lexer->line, lexer->column};
            motys_array_add(lexer->Tokens, &t);
        }
        else if (is_operator(c))
        {
        	Token t;
         	t.type = TOKEN_OPERATOR;
          	t.value = strndup(&lexer->source[lexer->position], 1);
           	t.line = lexer->line;
            	t.column = lexer->column;
        	motys_array_add(lexer->Tokens, &t);
         	lexer->position++;
        }
        else if (is_delimiter(c))
	{
		Token t;
	 	t.type = TOKEN_DELIMITER;
	  	t.value = strndup(&lexer->source[lexer->position], 1);
	   	t.line = lexer->line;
	    	t.column = lexer->column;
		motys_array_add(lexer->Tokens, &t);
	 	lexer->position++;
	}
        // Handle unrecognized characters
        else {
            // For now, just move past unknown characters
            printf("Unhandled character: %c\n", c);
            lexer->position++;
            lexer->column++;
        }
    }

    // Print tokens
    for (int i = 0; i < lexer->Tokens->size; i++) {
        Token *t = motys_array_get(lexer->Tokens, i);
        printf("Token %d: '%s'\t(Type: %d, Line: %d, Column: %d)\n", i + 1, t->value, t->type, t->line, t->column);
    }

}

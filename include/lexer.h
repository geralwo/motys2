#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "motys_stdlib.h"


typedef struct {
	char * source;
	int length;
	int position;
	int line;
	int column;
	MotysArray* tokens;
} Lexer;

char* strndup(const char* str, int n);

void lexer_init(Lexer* lexer, char* source);
void lexer_free(Lexer* lexer);
void lexer_tokenize(Lexer* _lexer);
//char* strndup(const char* str, size_t n);
#endif // LEXER_H

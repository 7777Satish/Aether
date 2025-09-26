#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "renderer.h"
#include "utils.h"

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_COMMENT,
    TOKEN_PUNCTUATION,
    TOKEN_WHITESPACE,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_PREPROCESSOR_DERIVATIVE
} TokenType;


typedef struct Token {
    char* content;
    SDL_Texture* t1;
    TokenType type;
    SDL_Color color;
    struct Token* next;
    struct Token* prev;
    int len;
} Token;

typedef struct FileLine
{
    // char *content;
    Token* word;
    SDL_Texture *t1;
    struct FileLine *next;
    struct FileLine *prev;
} FileLine;


typedef struct FileLine FileLine;

Token* createToken(char* word, int custom, SDL_Color color);
FileLine* parseText(char* content);

#endif
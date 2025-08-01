#include "parser.h"

// --- C Syntax Tokens for Highlighting ---

// Keywords (blue or bold cyan)
char *keywords[] = {
    "auto", "break", "case", "const",              // blue
    "continue", "default", "do", "else",           // blue
    "extern", "for", "goto",                       // blue
    "if", "inline", "register",                    // blue
    "restrict", "return", "short", "signed",       // blue
    "sizeof", "static", "struct", "switch",        // blue
    "typedef", "union", "unsigned", "void",        // blue
    "volatile", "while",                           // blue
    "_Alignas", "_Alignof", "_Atomic",             // blue
    "_Bool", "_Complex", "_Generic", "_Imaginary", // blue
    "_Noreturn", "_Static_assert", "_Thread_local" // blue
    ,
    NULL};

// Types (cyan or teal)
char *types[] = {
    "int", "float", "double", "char", "short", "enum",   // cyan
    "long", "unsigned", "signed", "void", "_Bool",       // cyan
    "_Complex", "_Imaginary", "char*", "int*", "float*", // cyan
    "long*", "short*", "unsigned*", "signed*", "void*", "bool", "bool*", "double*"
                                                                         "FILE*",
    "FILE", "DIR", "DIR*", NULL};

// Constants / Literals (orange or yellow)
char *constants[] = {
    "NULL", "true", "false" // orange (from stdbool.h)
    ,
    NULL};

// Preprocessor directives (purple or pink)
char *preprocessor_directives[] = {
    "#define", "#include", "#ifdef", "#ifndef", "#endif", // purple
    "#if", "#else", "#elif", "#undef", "#pragma",         // purple
    "#error", "#line"                                     // purple
    ,
    NULL};

// Operators (gray or no color)
char *operators[] = {
    "+", "-", "*", "/", "%", "++", "--", // gray
    "=", "+=", "-=", "*=", "/=", "%=",   // gray
    "==", "!=", ">", "<", ">=", "<=",    // gray
    "&&", "||", "!", "&", "|", "^", "~", // gray
    "<<", ">>", "<<=", ">>=",            // gray
    ".", "->", "?", ":", "::"            // gray
    ,
    NULL};

// Punctuation / Delimiters (white or light gray)
char *punctuation[] = {
    "(", ")", "{", "}", "[", "]", ";", "," // white
    ,
    NULL};

// Identifiers (default or white)
char *identifiers[] = {
    "main", "printf", "scanf", "myVar", "my_function" // white
    ,
    NULL};

int isKeyword(char *word)
{
    for (int i = 0; keywords[i]; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isType(char *word)
{
    for (int i = 0; types[i]; i++)
    {
        if (strcmp(word, types[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isConstant(char *word)
{
    for (int i = 0; constants[i]; i++)
    {
        if (strcmp(word, constants[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isPreprocessor_directive(char *word)
{
    for (int i = 0; preprocessor_directives[i]; i++)
    {
        if (strcmp(word, preprocessor_directives[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isOperator(char *word)
{
    for (int i = 0; operators[i]; i++)
    {
        if (strcmp(word, operators[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isPunctuation(char *word)
{
    for (int i = 0; punctuation[i]; i++)
    {
        if (strcmp(word, punctuation[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isIdentifier(char *word)
{
    for (int i = 0; identifiers[i]; i++)
    {
        if (strcmp(word, identifiers[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isNumber(char *word)
{
    int i = 0;
    while (word[i] != '\0')
    {
        if (!isdigit(word[i]))
            return 0;
        i++;
    }
    return 1;
}

TokenType getTokenType(char *word)
{
    if (isType(word))
        return TOKEN_TYPE;
    else if (isKeyword(word))
        return TOKEN_KEYWORD;
    else if (isIdentifier(word))
        return TOKEN_IDENTIFIER;
    else if (isConstant(word))
        return TOKEN_KEYWORD;
    else if (isOperator(word))
        return TOKEN_OPERATOR;
    else if (isPreprocessor_directive(word))
        return TOKEN_PREPROCESSOR_DERIVATIVE;
    else if (isPunctuation(word))
        return TOKEN_PUNCTUATION;
    else if (isNumber(word))
        return TOKEN_NUMBER;
    else
        return TOKEN_UNKNOWN;
}

Token *createToken(char *word, int custom, SDL_Color color)
{
    TokenType type = getTokenType(word);
    Token *t = (Token *)malloc(sizeof(Token));
    size_t size = strlen(word);
    t->content = malloc(size + 1);
    t->content[size] = '\0';
    strcpy(t->content, word);
    t->type = type;
    t->len = size;
    SDL_Color clr;
    if (custom)
    {
        clr = color;
    }
    else
    {
        if (type == TOKEN_TYPE)
            clr = (SDL_Color){198, 120, 221, 255};
        else if (type == TOKEN_KEYWORD)
            clr = (SDL_Color){198, 120, 221, 255};
        else if (type == TOKEN_IDENTIFIER)
            clr = (SDL_Color){220, 220, 170, 255};
        else if (type == TOKEN_OPERATOR)
            clr = (SDL_Color){212, 212, 212, 255};
        else if (type == TOKEN_PREPROCESSOR_DERIVATIVE)
            clr = (SDL_Color){155, 155, 255, 255};
        else if (type == TOKEN_PUNCTUATION)
            clr = (SDL_Color){128, 200, 255, 255};
        else if (type == TOKEN_NUMBER)
            clr = (SDL_Color){181, 206, 168, 255};
        else
            clr = (SDL_Color){255, 255, 255, 255};
    }

    SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, word, clr);
    t->t1 = SDL_CreateTextureFromSurface(renderer, s1);
    SDL_FreeSurface(s1);

    t->next = NULL;
    t->prev = NULL;

    return t;
}

FileLine *parseText(char *content)
{

    char c = content[0];
    int i = 0;

    char *currentWord = malloc(1);
    currentWord[0] = '\0';

    FileLine *lines = NULL;
    FileLine *prevLine = NULL;
    Token *lastToken = NULL;
    Token *firstToken = NULL;
    SDL_Color white = {255, 255, 255, 255};
    int isInString = 0;
    int isInComment = 0;

    while (c != '\0')
    {
        char tempC[2];
        tempC[0] = c;
        tempC[1] = '\0';

        if (c == '/' && content[i + 1] == '/')
        {
            isInComment = 1;
        }

        if (!isInComment && (c == '"' || c == '\''))
        {
            int quoteType = (c == '"') ? 1 : 2;

            if (isInString == quoteType)
            {
                // Check for escape
                int backslashCount = 0;
                int j = i - 1;
                while (j >= 0 && content[j] == '\\')
                {
                    backslashCount++;
                    j--;
                }

                if (backslashCount % 2 == 0)
                {

                    size_t len = strlen(currentWord);
                    currentWord = realloc(currentWord, len + 2);
                    currentWord[len] = c;
                    currentWord[len + 1] = '\0';

                    Token *t = createToken(currentWord, 1, (SDL_Color){206, 145, 120, 255});
                    t->type = TOKEN_STRING;
                    t->prev = lastToken;
                    t->next = NULL;
                    if (lastToken)
                        lastToken->next = t;
                    lastToken = t;
                    if (!firstToken)
                        firstToken = lastToken;

                    free(currentWord);
                    currentWord = malloc(1);
                    currentWord[0] = '\0';

                    isInString = 0;
                    i++;
                    c = content[i];
                    continue;
                }
            }
            else if (!isInString)
            {
                isInString = quoteType;
            }
        }

        if (!isInString && !isInComment && ((c == ' ') || isPunctuation(tempC) || isOperator(tempC)))
        {

            Token *t = NULL;

            if ((lastToken && strcmp(lastToken->content, " ") == 0) && (lastToken->prev && lastToken->prev->type == TOKEN_TYPE) && !isPunctuation(tempC))
            {
                t = createToken(currentWord, 1, (SDL_Color){200, 255, 200, 255});
            }
            else if (c == '(' && (strcmp(currentWord, " ") != 0) && !isKeyword(currentWord))
            {
                t = createToken(currentWord, 1, (SDL_Color){220, 220, 170, 255});
            }
            else
            {
                t = createToken(currentWord, 0, white);
            }

            t->prev = lastToken;
            t->next = NULL;

            if (lastToken)
                lastToken->next = t;
            lastToken = t;

            if (!firstToken)
            {
                firstToken = t;
                lastToken = t;
            }

            free(currentWord);
            currentWord = malloc(1);
            currentWord[0] = '\0';

            Token *t2 = NULL;
            if (c == ' ')
                t2 = createToken(tempC, 1, white);
            else
                t2 = createToken(tempC, 0, white);

            t2->prev = lastToken;
            t2->next = NULL;

            if (lastToken)
                lastToken->next = t2;
            lastToken = t2;

            i++;
            c = content[i];
            continue;
        }

        if (c == '\n')
        {
            Token *t = NULL;
            if (!isInComment)
            {
                t = createToken(currentWord, 0, white);
            }
            else if (isInComment == 1)
            {
                t = createToken(currentWord, 1, (SDL_Color){87, 166, 74, 255});
                t->type = TOKEN_COMMENT;
                isInComment = 0;
            }

            t->prev = lastToken;
            t->next = NULL;

            if (lastToken)
                lastToken->next = t;
            lastToken = t;

            if (!firstToken)
            {
                firstToken = t;
                lastToken = t;
            }

            free(currentWord);
            currentWord = malloc(1);
            currentWord[0] = '\0';

            FileLine *line = (FileLine *)malloc(sizeof(FileLine));

            line->word = firstToken;

            line->next = NULL;
            line->prev = prevLine;

            if (!lines)
            {
                lines = line;
            }
            else if (prevLine)
            {
                prevLine->next = line;
            }

            firstToken = NULL;
            lastToken = NULL;
            prevLine = line;

            i++;
            c = content[i];
            continue;
        }

        size_t len = strlen(currentWord);
        currentWord = realloc(currentWord, len + 2);
        currentWord[len] = c;
        currentWord[len + 1] = '\0';

        i++;
        c = content[i];
    }

    return lines;
}
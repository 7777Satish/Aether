#include "completion.h"

int showCompletion = 1;

CompletionBoxNode CompletionBox = {
    NULL, NULL, 0
};

char *C_SUGGESTIONS[] = {
    "#define",
    "#elif",
    "#else",
    "#endif",
    "#error",
    "#if",
    "#ifdef",
    "#ifndef",
    "#include",
    "#line",
    "#pragma",
    "#undef",

    "__DATE__",
    "__FILE__",
    "__LINE__",
    "__STDC__",
    "__STDC_HOSTED__",
    "__STDC_NO_ATOMICS__",
    "__STDC_NO_COMPLEX__",
    "__STDC_NO_THREADS__",
    "__STDC_NO_VLA__",
    "__STDC_VERSION__",
    "__TIME__",
    "__func__",

    "_Alignas",
    "_Alignof",
    "_Atomic",
    "_Bool",
    "_Complex",
    "_Generic",
    "_Imaginary",
    "_Noreturn",
    "_Static_assert",
    "_Thread_local",

    "DIR",
    "DIR*",
    "EOF",
    "EXIT_FAILURE",
    "EXIT_SUCCESS",
    "FILE",
    "FILE*",
    "NULL",

    "abort",
    "abs",
    "atexit",
    "atoi",
    "auto",

    "bool",
    "bool*",
    "break",

    "calloc",
    "case",
    "ceil",
    "char",
    "char*",
    "const",
    "continue",

    "default",
    "do",
    "double",
    "double*",

    "else",
    "enum",
    "exit",
    "extern",

    "false",
    "float",
    "float*",
    "floor",
    "for",
    "fclose",
    "fgetc",
    "fgets",
    "fopen",
    "fprintf",
    "fputc",
    "fputs",
    "fread",
    "freopen",
    "fscanf",
    "fseek",
    "ftell",
    "fwrite",
    "free",

    "getc",
    "getchar",
    "getenv",
    "goto",

    "if",
    "inline",
    "int",
    "int*",

    "long",
    "long*",

    "main",
    "malloc",
    "memcmp",
    "memcpy",
    "memmove",
    "memset",

    "pow",
    "printf",
    "ptrdiff_t",
    "putc",
    "putchar",
    "puts",

    "qsort",

    "rand",
    "realloc",
    "register",
    "restrict",
    "return",

    "scanf",
    "short",
    "short*",
    "signed",
    "signed*",
    "sizeof",
    "size_t",
    "sqrt",
    "ssize_t",
    "static",
    "stderr",
    "stdin",
    "stdout",
    "strcat",
    "strchr",
    "strcmp",
    "strcpy",
    "strlen",
    "strncat",
    "strncmp",
    "strncpy",
    "strstr",
    "struct",
    "switch",
    "system",

    "true",
    "typedef",

    "union",
    "unsigned",
    "unsigned*",

    "void",
    "void*",
    "volatile",

    "while",

    NULL
};


int startsWith(char *string, char *sub, int b)
{
    if (!string || !sub)
        return 0;

    int a = (int)strlen(string);

    if (a < b)
        return 0;

    int i = 0;
    while (i < b)
    {
        if (string[i] != sub[i])
            return 0;
        i++;
    }

    return 1;
}

void freeCompletionList()
{
    CompletionListItem *item = CompletionBox.list;
    while (item)
    {
        SDL_DestroyTexture(item->t1);
        CompletionListItem *temp = item;
        item = item->next;
        free(temp);
    }
}

int getCompletion(char *word, int n)
{
    if(n==0){
        showCompletion = 0;
        return;
    };
    int i = 0;

    freeCompletionList();

    CompletionListItem *head = NULL;
    CompletionListItem *item = NULL;

    while (C_SUGGESTIONS[i])
    {
        if (startsWith(C_SUGGESTIONS[i], word, n))
        {
            CompletionListItem *node = (CompletionListItem *)malloc(sizeof(CompletionListItem));
            node->text = C_SUGGESTIONS[i];
            SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->text, (SDL_Color){230, 230, 230, 255});
            node->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);
            node->next = NULL;
            node->prev = item;

            if (!item)
            {
                item = node;
            }
            else
            {
                item->next = node;
            }

            if (!head)
            {
                head = node;
            }

            item = node;
        }
        i++;
    }

    if (i > 0)
        showCompletion = 1;
    else
        showCompletion = 0;
    
    CompletionBox.list = head;
    CompletionBox.active = head;
    CompletionBox.tail = item;
    CompletionBox.scrollY = 0;
}
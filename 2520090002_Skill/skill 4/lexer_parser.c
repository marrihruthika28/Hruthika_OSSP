#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_LEN 50

typedef enum {
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_LEN];
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;

/* Convert token type to readable text */
const char* tokenName(TokenType type) {
    switch (type) {
        case WORD:        return "WORD";
        case PIPE:        return "PIPE";
        case REDIRECT_IN: return "REDIRECT_IN";
        case REDIRECT_OUT:return "REDIRECT_OUT";
        case END:         return "END";
        default:          return "UNKNOWN";
    }
}

/* Lexer */
void tokenize(char *input) {

    int i = 0;

    while (input[i] != '\0' && token_count < MAX_TOKENS - 1) {

        /* Handle whitespace */
        if (isspace(input[i])) {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|') {
            tokens[token_count].type = PIPE;
            strcpy(tokens[token_count].value, "|");
            token_count++;
            i++;
        }

        /* Input redirection */
        else if (input[i] == '<') {
            tokens[token_count].type = REDIRECT_IN;
            strcpy(tokens[token_count].value, "<");
            token_count++;
            i++;
        }

        /* Output redirection */
        else if (input[i] == '>') {
            tokens[token_count].type = REDIRECT_OUT;
            strcpy(tokens[token_count].value, ">");
            token_count++;
            i++;
        }

        /* Word */
        else {
            int j = 0;

            while (input[i] != '\0' &&
                   !isspace(input[i]) &&
                   input[i] != '|' &&
                   input[i] != '<' &&
                   input[i] != '>') {

                if (j < MAX_LEN - 1) {
                    tokens[token_count].value[j++] = input[i];
                }

                i++;
            }

            tokens[token_count].value[j] = '\0';
            tokens[token_count].type = WORD;
            token_count++;
        }
    }

    tokens[token_count].type = END;
    strcpy(tokens[token_count].value, "END");
    token_count++;
}

/* Display tokens */
void printTokens() {

    printf("\n--- TOKENS ---\n");

    for (int i = 0; i < token_count; i++) {
        printf("%d : %-15s %s\n",
               i,
               tokenName(tokens[i].type),
               tokens[i].value);
    }
}

/* Parser */
int parse() {

    int i = 0;
    int expectWord = 1;

    if (tokens[0].type == END) {
        printf("\nEmpty command.\n");
        return 0;
    }

    while (tokens[i].type != END) {

        if (expectWord) {

            if (tokens[i].type != WORD) {
                printf("\nSyntax Error: Expected a command/word near '%s'\n",
                       tokens[i].value);
                return 0;
            }

            printf("Command/Argument: %s\n", tokens[i].value);
            expectWord = 0;
        }

        else {

            if (tokens[i].type == WORD) {
                printf("Argument: %s\n", tokens[i].value);
            }

            else if (tokens[i].type == PIPE) {

                printf("Pipe detected: |\n");
                expectWord = 1;
            }

            else if (tokens[i].type == REDIRECT_IN) {

                printf("Input redirection: <\n");

                i++;

                if (tokens[i].type != WORD) {
                    printf("\nSyntax Error: File name expected after '<'\n");
                    return 0;
                }

                printf("Input file: %s\n", tokens[i].value);
            }

            else if (tokens[i].type == REDIRECT_OUT) {

                printf("Output redirection: >\n");

                i++;

                if (tokens[i].type != WORD) {
                    printf("\nSyntax Error: File name expected after '>'\n");
                    return 0;
                }

                printf("Output file: %s\n", tokens[i].value);
            }
        }

        i++;
    }

    if (expectWord) {
        printf("\nSyntax Error: Command expected after pipe.\n");
        return 0;
    }

    printf("\nParsing successful.\n");
    return 1;
}

int main() {

    char input[500];

    printf("Enter a command: ");
    fgets(input, sizeof(input), stdin);

    /* Remove newline */
    input[strcspn(input, "\n")] = '\0';

    tokenize(input);

    printTokens();

    printf("\n--- PARSER OUTPUT ---\n");

    parse();

    return 0;
}

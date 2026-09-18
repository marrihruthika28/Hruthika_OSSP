#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS 100
#define MAX_LEN 200

typedef struct {
    char value[MAX_LEN];
    int quoted;
    char quoteType;
} Token;

Token tokens[MAX_TOKENS];
int tokenCount = 0;

/* Add a token */
void addToken(char *text, int quoted, char quoteType) {

    if (tokenCount >= MAX_TOKENS)
        return;

    strcpy(tokens[tokenCount].value, text);
    tokens[tokenCount].quoted = quoted;
    tokens[tokenCount].quoteType = quoteType;

    tokenCount++;
}

/* Expand environment variable */
void expandVariable(char *input, char *output) {

    char variable[100];
    int i = 0, j = 0;

    while (input[i] != '\0') {

        if (input[i] == '$') {

            i++;
            j = 0;

            while ((input[i] >= 'A' && input[i] <= 'Z') ||
                   (input[i] >= 'a' && input[i] <= 'z') ||
                   (input[i] >= '0' && input[i] <= '9') ||
                   input[i] == '_') {

                variable[j++] = input[i++];
            }

            variable[j] = '\0';

            char *value = getenv(variable);

            if (value != NULL) {
                strcat(output, value);
            }
        }

        else {
            int len = strlen(output);
            output[len] = input[i];
            output[len + 1] = '\0';
            i++;
        }
    }
}

/* Tokenizer with quote handling */
int tokenize(char *input) {

    int i = 0;

    while (input[i] != '\0') {

        /* Ignore spaces outside quotes */
        if (input[i] == ' ' ||
            input[i] == '\t' ||
            input[i] == '\n') {
            i++;
            continue;
        }

        /* SINGLE QUOTES */
        if (input[i] == '\'') {

            char buffer[MAX_LEN] = "";
            int j = 0;

            i++;

            while (input[i] != '\0' && input[i] != '\'') {

                if (j < MAX_LEN - 1)
                    buffer[j++] = input[i];

                i++;
            }

            if (input[i] == '\0') {
                printf("\nError: Unclosed single quote.\n");
                return 0;
            }

            buffer[j] = '\0';

            addToken(buffer, 1, '\'');

            i++;
        }

        /* DOUBLE QUOTES */
        else if (input[i] == '"') {

            char buffer[MAX_LEN] = "";
            char expanded[MAX_LEN] = "";
            int j = 0;

            i++;

            while (input[i] != '\0' && input[i] != '"') {

                if (j < MAX_LEN - 1)
                    buffer[j++] = input[i];

                i++;
            }

            if (input[i] == '\0') {
                printf("\nError: Unclosed double quote.\n");
                return 0;
            }

            buffer[j] = '\0';

            /*
             * Variable expansion is allowed
             * inside double quotes.
             */
            expandVariable(buffer, expanded);

            addToken(expanded, 1, '"');

            i++;
        }

        /* NORMAL TOKEN */
        else {

            char buffer[MAX_LEN] = "";
            int j = 0;

            while (input[i] != '\0' &&
                   input[i] != ' ' &&
                   input[i] != '\t' &&
                   input[i] != '\n' &&
                   input[i] != '\'' &&
                   input[i] != '"') {

                if (j < MAX_LEN - 1)
                    buffer[j++] = input[i];

                i++;
            }

            buffer[j] = '\0';

            if (strlen(buffer) > 0)
                addToken(buffer, 0, '\0');
        }
    }

    return 1;
}

/* Display parsed tokens */
void displayTokens() {

    printf("\n========== PARSING RESULT ==========\n");

    for (int i = 0; i < tokenCount; i++) {

        printf("Token %d: ", i + 1);

        if (tokens[i].quoted) {

            if (tokens[i].quoteType == '\'')
                printf("SINGLE QUOTED");

            else
                printf("DOUBLE QUOTED");

        } else {
            printf("NORMAL");
        }

        printf(" -> [%s]\n", tokens[i].value);
    }
}

/* Main function */
int main() {

    char input[1000];

    printf("====================================\n");
    printf("   SINGLE & DOUBLE QUOTE PARSER\n");
    printf("====================================\n");

    printf("\nEnter command: ");

    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) {

        printf("\nEmpty command.\n");
        return 0;
    }

    if (tokenize(input)) {

        displayTokens();

        printf("\n========== VALIDATION ==========\n");
        printf("Parsing successful.\n");

        printf("\nExecution structure:\n");

        for (int i = 0; i < tokenCount; i++) {
            printf("Argument[%d] = %s\n",
                   i,
                   tokens[i].value);
        }
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SYMBOLS 64
#define MAX_STACK_SIZE 65536
struct Frame {
    char symbol;
    int row;
    int column;
};

enum {
    SUCCESS = 0,
    UNBALANCED = 1,
    ERROR = 2
};

void handle_args(int argc, char *argv[]);
void stack_push(struct Frame);
struct Frame stack_pop();
void terminate(int code);
void show_help();

struct Frame stack[MAX_STACK_SIZE];
int stack_index = -1;
bool symarg = false;
bool filearg = false;
char symbols[MAX_SYMBOLS + 1] = {0};
char filename[255] = {0};
FILE *file;

int main(int argc, char *argv[]) {
    
    handle_args(argc, argv); 

    

    if (filearg == false) {
        file = stdin;
    } else {
        file = fopen(filename, "r");
    }

    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        terminate(ERROR);
    }

    int ch;
    int row = 1;
    int column = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            row++;
            column = 0;
            continue;
        }
        column++;

        char *loc = strchr(symbols, ch);
        if (loc != NULL) { // symbol found
            const int index = loc - symbols;
            if (index % 2 == 0) { // opening symbol
                const struct Frame frame = {ch, row, column};
                stack_push(frame);
            } else {  // closing symbol
                if (stack_index == -1 || stack_pop().symbol != symbols[index - 1]) {
                    printf("Closing symbol on %d:%d has no match ---> %c\n", row, column, ch);
                    terminate(UNBALANCED);
                }
            }
        }
    }

    if (stack_index != -1) {
        const struct Frame frame = stack_pop();
        printf("Opening symbol on %d:%d has no match ---> %c\n", frame.row, frame.column, frame.symbol);
        terminate(UNBALANCED);
    }
    
    terminate(SUCCESS);
}

void terminate(const int code) {
    if (filearg == true && file != NULL) {
        fclose(file);
    }
    exit(code);
}

void stack_push(const struct Frame frame) {
    if (stack_index == MAX_STACK_SIZE - 1) {
        printf("Stack full\n");
        terminate(ERROR);
    }
    stack[++stack_index] = frame;
}

struct Frame stack_pop() {
    if (stack_index == -1) {
        printf("Stack empty\n");
        terminate(ERROR);
    }
    return stack[stack_index--];
}

void handle_args(const int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf("version 0\n");
            terminate(SUCCESS);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            show_help();
            terminate(SUCCESS);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            if (symarg == true) {
                printf("Please provide all symbols with a single -s argument\n");
                terminate(ERROR);
            }
            if (strlen(argv[i + 1]) % 2 != 0) {
                printf("Invalid format. Provide an even number of opening and closing symbols\n");
                terminate(ERROR);
            }
            if (strlen(argv[i + 1]) > MAX_SYMBOLS) {
                printf("You may only define up to %d symbols\n", MAX_SYMBOLS);
                terminate(ERROR);
            }
            memcpy(symbols, argv[i + 1], strlen(argv[i + 1]));
            symarg = true;
            i++;
        } else {
            if (filearg == true) {
                printf("You provided more than one file\n");
                terminate(ERROR);
            }
            memcpy(filename, argv[i], strlen(argv[i]));
            filearg = true;
        }
    }

    if (symarg == false) {
        memcpy(symbols, "[]{}()", 6);
    }
}

void show_help() {
    printf("Usage: brace [OPTION]... [FILE]\n");
    printf("Find unbalanced symbols in text.\n\n");
    printf("With no FILE, read standard input.\n\n");
    printf("  -s <symbols>          define custom pairs of open/close symbols\n");
    printf("                        (default is \"[]{}()\"\n");
    printf("  -v, --version         show current version\n");
    printf("  -h, --help            display this help and exit\n\n");
    printf("Example:\n");
    printf("brace -s \"()<>\" main.c\n\n");
}

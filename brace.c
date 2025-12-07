#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SYMBOLS 64

struct Frame {
    char symbol;
    int row;
    int column;
};

void handle_args(int argc, char *argv[]);
void stack_add(struct Frame);
void stack_push(struct Frame);
struct Frame stack_pop();
void terminate(int code, FILE *file);
void show_help();

struct Frame stack[65536];
int stack_index = -1;
bool symarg = false;
bool filearg = false;
char symbols[MAX_SYMBOLS + 1] = {0};
size_t symbol_size = 0;
char filename[255] = {0};
char buffer[2048];

int main(int argc, char *argv[]) {
    
    handle_args(argc, argv); 

    
    FILE *file;

    if (filearg == false) {
        file = stdin;
    } else {
        file = fopen(filename, "r");
    }

    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        terminate(1, NULL);
    }

    char ch;
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
            int index = loc - symbols;
            if (index % 2 == 0) { // opening symbol
                struct Frame frame = {ch, row, column};
                stack_push(frame);
            } else {  // closing symbol
                if (stack_index == -1 || (stack_pop()).symbol != symbols[index - 1]) { 
                    printf("Closing symbol on %d:%d has no match ---> %c\n", row, column, ch);
                    terminate(1, file);
                }
            }
        }
    }

    if (stack_index != -1) {
        struct Frame frame = stack_pop();
        printf("Opening symbol on %d:%d has no match ---> %c\n", frame.row, frame.column, frame.symbol);
        terminate(1, file);
    }
    
    fclose(file);
    return 0;
}

void terminate(int code, FILE *file) {
    if (filearg == true && file != NULL) {
        fclose(file);
    }
    exit(code);
}

void stack_push(struct Frame frame) {
    stack[stack_index++] = frame;
}

struct Frame stack_pop() {
    return stack[--stack_index];
}

void handle_args(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf("version 0\n");
            terminate(0, NULL);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            show_help();
            terminate(0, NULL);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            if (symarg == true) {
                printf("Please provide all symbols with a single -s argument\n");
                terminate(1, NULL);
            }
            if (strlen(argv[i + 1]) % 2 != 0) {
                printf("Invalid format. Provide an even number of opening and closing symbols\n");
                terminate(1, NULL);
            }
            if (strlen(argv[i + 1]) > MAX_SYMBOLS) {
                printf("You may only define up to %d symbols\n", MAX_SYMBOLS);
                terminate(1, NULL);
            }
            memcpy(symbols, argv[i + 1], strlen(argv[i + 1]));
            symarg = true;
            i++;
        } else {
            if (filearg == true) {
                printf("You provided more than one file\n");
                terminate(1, NULL);
            }
            memcpy(filename, argv[i], strlen(argv[i]));
            filearg = true;
        }
    }

    if (symarg == false) {
        memcpy(symbols, "[]{}()", 6);
    }
}



/*
Usage: cat [OPTION]... [FILE]...
Concatenate FILE(s) to standard output.

With no FILE, or when FILE is -, read standard input.

  -A, --show-all           equivalent to -vET
  -b, --number-nonblank    number nonempty output lines, overrides -n
  -e                       equivalent to -vE
  -E, --show-ends          display $ at end of each line
  -n, --number             number all output lines
  -s, --squeeze-blank      suppress repeated empty output lines
  -t                       equivalent to -vT
  -T, --show-tabs          display TAB characters as ^I
  -u                       (ignored)
  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB
      --help        display this help and exit
      --version     output version information and exit

Examples:
  cat f - g  Output f's contents, then standard input, then g's contents.
  cat        Copy standard input to standard output.

*/
void show_help() {
    printf("Usage: brace [OPTION]... [FILE]\n");
    printf("Find unbalanced symbols in text.\n\n");
    printf("With no FILE, read standard input.\n\n");
    printf("  -s <symbols>          define custom opening and closing symbols\n");
    printf("  -v, --version         show current version\n");
    printf("  -h, --help            display this help and exit\n");
    printf("\n");
    printf("Example:\n");
    printf("brace -s \"()<>\" main.c\n\n");
}

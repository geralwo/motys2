#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "vm.h"

// Function to read the entire file into a string
char* read_file_to_string(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Seek to the end to determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file); // Go back to the start of the file

    // Allocate memory for the file contents + null terminator
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file contents into the buffer
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0'; // Null-terminate the string

    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    char* source_code = read_file_to_string(argv[1]);
    if (!source_code) {
        return 1;
    }

    printf("Source Code:\n----\n%s\n----\n", source_code);

    Lexer lexer;
    lexer_init(&lexer, source_code);
    lexer_tokenize(&lexer);

    MotysVM vm;
    motys_vm_init(&vm);

    // Free allocated memory
    free(source_code);
    lexer_free(&lexer);


    return 0;
}

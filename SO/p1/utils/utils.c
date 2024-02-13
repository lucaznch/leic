#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define UINT_MAX_CHAR_NUMBER 11 /// the max size that a uint can be in characters (including the null terminator)

char* filename_extension_changer(const char *filename, const char *new_extension) {
    const char *dot = strrchr(filename, '.'); /// find the dot in the filename
    size_t length_without_extension;

    if (dot != NULL) {
        length_without_extension = (size_t) (dot - filename); /// distance between the dot and the start of the filename
    } else {
        length_without_extension = strlen(filename); /// if there's no dot for some reason, the length is the length of the filename
    }

    char *output = (char*) malloc(length_without_extension + strlen(new_extension) + 1); // allocate memory for the output string
    if (output == NULL) {
        fprintf(stderr, "Error: Memory allocation error while allocating memory for the output filename string.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(output, filename, length_without_extension); /// copy the filename without the extension
    strcpy(output + length_without_extension, new_extension); /// add the new extension to the end of the filename
    
    return output;
}

char* uint_to_string(unsigned int num) {
    char* str = (char*) malloc(UINT_MAX_CHAR_NUMBER * sizeof(char)); /// allocate memory for the uint string

    if (str == NULL) { /// if the memory allocation fails
        fprintf(stderr, "Error: Memory allocation error while allocating memory for a unsigned int as a string.\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    if (num == 0) { /// if the uint is 0
        str[index++] = '0';
    }

    while (num > 0) { /// for extracting the digits of the uint (in reverse order)
        unsigned int digit = num % 10;
        str[index++] = (char)(digit + '0');
        num /= 10;
    }

    str[index] = '\0'; /// for the string to be null-terminated

    int start = 0; /// reset the start for reversing the string
    int end = index - 1; /// reset the end for reversing the string

    while (start < end) { /// for reversing the string
        char temp = str[start]; /// to help with the swapping
        str[start] = str[end];
        str[end] = temp;

        start++;
        end--;
    }

    return str;
}
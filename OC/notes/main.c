#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_LINE_SIZE 32
#define MAX_LINE_SIZE_CONV_CMD 20 /* 10 digits max */
#define MAX_LINE_SIZE_OP_CMD 40
#define MAX_NUMBER 524287

#define CONVERT_DEC 2
#define CONVERT_BIN 3
#define CONVERT_HEX 4

#define CMD_CONVERT 5
#define CMD_OPERATION 6
#define CMD_EOL 7
#define CMD_EXIT 8
#define CMD_INVALID 9

#define ADD '+'
#define SUB '-'
#define MUL '*'
#define MOD '%'
#define DIV '/'
#define AND '&'
#define OR '|'
#define XOR '^'
#define NOT '~'
#define SHL '<'
#define SHR '>'

#define START_SCREEN "\033[0;33mcommands:\n\t/convbin <binary>\t\t/opbin <operator> <binary1> <binary2>\n\t/convhex <hexadecimal>\t\t/ophex <operator> <hex1> <hex2>\n\t/convdec <decimal>\n\t/exit\t\t\t\033[0;36moperators: +, -, *, /, %%, ~, &, |, ^, >, <.\n\n\033[0m"



/*
converts a long into binary
@param k number to convert to binary
*/
long in_binary(long k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    return (k % 2) + 10 * in_binary(k / 2);
}


/*
converts the number into binary, decimal or hexadecimal
@param type type of conversion
@param number original number
*/
void converter(int type, long n) {
    char *green = "\033[0;32m";
    char *reset = "\033[0m";

    switch (type) {
        case CONVERT_DEC:
            if (n > MAX_NUMBER) { printf("decimal received: %s%ld%s\n\tbinary: %s%s%s\t\thexadecimal: %s%lX%s\n\n", green, n, reset, green, "too big", reset, green, n, reset); }
            else { printf("decimal received: %s%ld%s\n\tbinary: %s%ld%s\t\thexadecimal: %s%lX%s\n\n", green, n, reset, green, in_binary(n), reset, green, n, reset); }
            break;
        
        case CONVERT_BIN:
            if (n > MAX_NUMBER) { printf("binary received: %s%s%s\n\tdecimal: %s%ld%s\t\thexadecimal: %s%lX%s\n\n", green, "too big", reset, green, n, reset, green, n, reset); }
            else { printf("binary received: %s%ld%s\n\tdecimal: %s%ld%s\t\thexadecimal: %s%lX%s\n\n", green, in_binary(n), reset, green, n, reset, green, n, reset); }
            break;
        
        case CONVERT_HEX:
            if (n > MAX_NUMBER) { printf("hexadecimal received: %s%lX%s\n\tdecimal: %s%ld%s\t\tbinary: %s%s%s\n\n", green, n, reset, green, n, reset, green, "too big", reset); }
            else { printf("hexadecimal received: %s%lX%s\n\tdecimal: %s%ld%s\t\tbinary: %s%ld%s\n\n", green, n, reset, green, n, reset, green, in_binary(n), reset); }
            break;
        
        default:
            break;
    }
}




void calculate(char operation, long n1, long n2) {

    switch (operation)
    {
    case ADD:
        printf("%ld + %ld = %ld\n\n", in_binary(n1), in_binary(n2), in_binary(n1 + n2));
        break;
    
    default:
        break;
    }



}



/*
reads until a newline character is found, 
the newline character is read, 
suggested before calling this function to check if the previous byte is the newline
@param fd file descriptor to read from
*/
void cleanup(int fd) {
  char ch;
  while (read(fd, &ch, 1) == 1 && ch != '\n') { ; }
}


/*
reads a line and processes it
@param fd file descriptor to read from
@return the command read
*/
int process_line(int fd) {
    char ch, *digits, *digits2, *endptr, buf[BUF_LINE_SIZE];
    int n, aux;
    long number, number2;

    if (read(fd, buf, 1) != 1) { return CMD_EOL; }

    if (buf[0] != '/') { /* not a command */
        if (buf[0] != '\n') { cleanup(fd); }
        return CMD_INVALID;
    }

    if (read(fd, buf + 1, 1) != 1) { return CMD_INVALID; }

    switch (buf[1]) {
        
        case 'c': 
            if ((n = read(fd, buf + 2, 3)) != 3 || strncmp(buf, "/conv", 5) != 0) {
                if (n == 3 && buf[4] != '\n') { cleanup(fd); }
                return CMD_INVALID;
            }
            if (read(fd, buf + 5, 1) != 1) { return CMD_INVALID; }
            switch (buf[5]) {
                case 'd':
                    if ((n = read(fd, buf + 6, 2)) != 2 || strncmp(buf, "/convdec", 8) != 0) {
                        if (n == 2 && buf[7] != '\n') { cleanup(fd); } 
                        return CMD_INVALID;
                    }
                    if (read(fd, buf + 8, 1) != 0 && buf[8] != ' ') { 
                        if (buf[8] != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }
                    n = 9;
                    while (n < MAX_LINE_SIZE_CONV_CMD && read(fd, &ch, 1) == 1 && ch >= 48 && ch <= 57) { buf[n++] = ch; }
                    if (ch != '\n') { cleanup(fd); }

                    if (n == 9) { return CMD_INVALID; }
                    else if (n == MAX_LINE_SIZE_CONV_CMD) { buf[n - 1] = '\0'; }
                    else {buf[n] = '\0';}

                    digits = &buf[9];
                    number = strtol(digits, &endptr, 10);

                    converter(CONVERT_DEC, number);
                    return CMD_CONVERT;
                case 'b':
                     if ((n = read(fd, buf + 6, 2)) != 2 || strncmp(buf, "/convbin", 8) != 0) {
                        if (n == 2 && buf[7] != '\n') { cleanup(fd); } 
                        return CMD_INVALID;
                    }
                    if (read(fd, buf + 8, 1) != 0 && buf[8] != ' ') { 
                        if (buf[8] != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }
                    n = 9;
                    while (n < MAX_LINE_SIZE_CONV_CMD && read(fd, &ch, 1) == 1 && ch >= 48 && ch <= 49) { buf[n++] = ch; }
                    if (ch != '\n') { cleanup(fd); }

                    if (n == 9) { return CMD_INVALID; }
                    else if (n == MAX_LINE_SIZE_CONV_CMD) { buf[n - 1] = '\0'; }
                    else {buf[n] = '\0';}

                    digits = &buf[9];
                    number = strtol(digits, &endptr, 2);

                    converter(CONVERT_BIN, number);
                    return CMD_CONVERT;
                case 'h':
                    if ((n = read(fd, buf + 6, 2)) != 2 || strncmp(buf, "/convhex", 8) != 0) {
                        if (n == 2 && buf[7] != '\n') { cleanup(fd); } 
                        return CMD_INVALID;
                    }
                    if (read(fd, buf + 8, 1) != 0 && buf[8] != ' ') { 
                        if (buf[8] != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }
                    n = 9;
                    while (n < MAX_LINE_SIZE_CONV_CMD && read(fd, &ch, 1) == 1 && ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 70))) { buf[n++] = ch; }
                    if (ch != '\n') { cleanup(fd); }

                    if (n == 9) { return CMD_INVALID; }
                    else if (n == MAX_LINE_SIZE_CONV_CMD) { buf[n - 1] = '\0'; }
                    else {buf[n] = '\0';}

                    digits = &buf[9];
                    number = strtol(digits, &endptr, 16);

                    converter(CONVERT_HEX, number);
                    return CMD_CONVERT;
                case '\n':
                    return CMD_INVALID;
                default:
                    cleanup(fd);
                    return CMD_INVALID;
            }
        
        case 'o':
            if ((n = read(fd, buf + 2, 1)) != 1 || strncmp(buf, "/op", 3) != 0) {
                if (n == 1 && buf[2] != '\n') { cleanup(fd); }
                return CMD_INVALID;
            }
            if (read(fd, buf + 3, 1) != 1) { return CMD_INVALID; }
            switch (buf[3]) {
                case 'b':
                    if ((n = read(fd, buf + 4, 2)) != 2 || strncmp(buf, "/opbin", 6) != 0) {
                        if (n == 2 && buf[5] != '\n') { cleanup(fd); } 
                        return CMD_INVALID;
                    }
                    if (read(fd, buf + 6, 1) != 0 && buf[6] != ' ') { 
                        if (buf[6] != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }

                    if (read(fd, buf + 7, 1) != 0 && buf[7] != ADD && buf[7] != SUB && buf[7] != SHL && buf[7] != SHR) {
                        if (buf[7] != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }

                    if (read(fd, buf + 6, 1) != 0 && buf[6] != ' ') { 
                        if (buf[6] != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }

                    n = 9;
                    while (n < MAX_LINE_SIZE_OP_CMD && read(fd, &ch, 1) == 1 && ch >= 48 && ch <= 49) { buf[n++] = ch; }
                    if (ch != ' ' || n == 9) {
                        if (ch != '\n') { cleanup(fd); }
                        return CMD_INVALID;
                    }

                    n++;
                    aux = n;
                    while (n < MAX_LINE_SIZE_OP_CMD && read(fd, &ch, 1) == 1 && ch >= 48 && ch <= 49) { buf[n++] = ch; }
                    if (ch != '\n') { cleanup(fd); }

                    if (n == aux) { return CMD_INVALID; }
                    else if (n == MAX_LINE_SIZE_OP_CMD) { buf[n - 1] = '\0'; }
                    else {buf[n] = '\0';}

                    buf[aux - 1] = '\0';

                    digits = &buf[9];
                    digits2 = &buf[aux];

                    number = strtol(digits, &endptr, 2);
                    number2 = strtol(digits2, &endptr, 2);
                    calculate(buf[7], number, number2);
                    
                    return CMD_OPERATION;
                case '\n':
                    return CMD_INVALID;
                default:
                    cleanup(fd);
                    return CMD_INVALID;
            }
            
            
            
            return CMD_EXIT;

        case 'e':
            if ((n = read(fd, buf + 2, 3)) != 3 || strncmp(buf, "/exit", 5) != 0) {
                if (n == 3 && buf[4] != '\n') { cleanup(fd); } /* if does not match the command but it reads the N bytes we check if last one is '\n' and if not we cleanpup*/
                return CMD_INVALID;
            }
            if (read(fd, buf + 5, 1) != 0 && buf[5] != '\n') { /* the command was read */
                cleanup(fd);
                return CMD_INVALID;
            }
            return CMD_EXIT;
        case '\n':
            return CMD_INVALID;
        default:
            cleanup(fd);
            return CMD_INVALID;
    } 
}


int main() {
    printf(START_SCREEN);
    while (1) {
        printf("> ");
        fflush(stdout);
        switch (process_line(STDIN_FILENO)) {
            case CMD_EXIT:
                return 0;
            case CMD_INVALID:
                printf("\033[31minvalid command!\n\n\033[0m");
                break;
            default:
                break;
        }
    }
    return 0;
}



/*

/convdec 1234 -> dec+bin+hex
/convhex 3CA -> dec+bin+hex
/convbin 000101010 -> dec+bin+hex

/opbin < 0110 2100
/ophex + 3CA 4CA

+, -, *, /, %, ~, &, |, ^, >, <.

arithmetic op: addition +, subtraction -, multiplication *, modulo %, division /.
bitwise op: and &, or |, xor ^, not ~, left shift <<, right shift >>

*/
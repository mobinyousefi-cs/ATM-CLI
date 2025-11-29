/*
 * Project:   Command-Line ATM Interface
 * File:      ui.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Console UI helper functions for input/output formatting, including
 *   ANSI colors and masked input for PINs.
 */

#include "ui.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)
#  include <conio.h>
#else
#  include <termios.h>
#  include <unistd.h>
#endif

void ui_print_line(void) {
    printf("--------------------------------------------------\n");
}

void ui_print_banner(void) {
    ui_print_line();
    printf(CLR_CYAN "  Command-Line ATM Interface\n" CLR_RESET);
    ui_print_line();
}

void ui_print_error(const char *message) {
    fprintf(stderr, CLR_RED "[ERROR] %s\n" CLR_RESET,
            message ? message : "(unknown error)");
}

void ui_print_status(const char *message) {
    printf(CLR_GREEN "[INFO] %s\n" CLR_RESET,
           message ? message : "");
}

int ui_read_line(char *buffer, size_t size) {
    if (!buffer || size == 0) {
        return 0;
    }

    if (!fgets(buffer, (int)size, stdin)) {
        return 0;
    }

    /* Strip trailing newline */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return 1;
}

int ui_read_int(const char *prompt, int *out_value) {
    if (!out_value) return 0;

    char line[MAX_LINE_LEN];
    for (;;) {
        if (prompt) {
            printf("%s", prompt);
        }

        if (!ui_read_line(line, sizeof(line))) {
            return 0;
        }

        if (line[0] == '\0') {
            continue;
        }

        char *endptr = NULL;
        errno = 0;
        long v = strtol(line, &endptr, 10);
        if (errno == 0 && endptr && *endptr == '\0') {
            *out_value = (int)v;
            return 1;
        }

        ui_print_error("Invalid integer. Please try again.");
    }
}

int ui_read_double(const char *prompt, double *out_value) {
    if (!out_value) return 0;

    char line[MAX_LINE_LEN];
    for (;;) {
        if (prompt) {
            printf("%s", prompt);
        }

        if (!ui_read_line(line, sizeof(line))) {
            return 0;
        }

        if (line[0] == '\0') {
            continue;
        }

        char *endptr = NULL;
        errno = 0;
        double v = strtod(line, &endptr);
        if (errno == 0 && endptr && *endptr == '\0') {
            *out_value = v;
            return 1;
        }

        ui_print_error("Invalid amount. Please enter a numeric value.");
    }
}

int ui_read_string(const char *prompt, char *buffer, size_t size) {
    if (!buffer || size == 0) return 0;

    if (prompt) {
        printf("%s", prompt);
    }
    return ui_read_line(buffer, size);
}

int ui_read_masked(const char *prompt, char *buffer, size_t size) {
    if (!buffer || size == 0) return 0;

    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }

#if defined(_WIN32) || defined(_WIN64)

    size_t idx = 0;
    int ch;

    for (;;) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            break;
        }

        if ((ch == '\b' || ch == 127) && idx > 0) {
            idx--;
            printf("\b \b");
            fflush(stdout);
            continue;
        }

        if (idx < size - 1 && ch != '\r' && ch != '\n') {
            buffer[idx++] = (char)ch;
            printf("*");
            fflush(stdout);
        }
    }

    printf("\n");
    buffer[idx] = '\0';
    return 1;

#else
    struct termios oldt, newt;
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        return 0;
    }
    newt = oldt;
    newt.c_lflag &= ~(ECHO);      /* disable echo */
    newt.c_lflag &= ~(ICANON);    /* disable canonical mode */
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        return 0;
    }

    size_t idx = 0;
    int ch;
    for (;;) {
        ch = getchar();
        if (ch == '\n' || ch == '\r') {
            break;
        }

        if ((ch == '\b' || ch == 127) && idx > 0) {
            idx--;
            printf("\b \b");
            fflush(stdout);
            continue;
        }

        if (ch == EOF) {
            break;
        }

        if (idx < size - 1) {
            buffer[idx++] = (char)ch;
            printf("*");
            fflush(stdout);
        }
    }

    buffer[idx] = '\0';
    printf("\n");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 1;
#endif
}

/*
 * Project:   Command-Line ATM Interface
 * File:      ui.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Console UI helper functions for input/output formatting.
 */

#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void ui_print_banner(void) {
    ui_print_line();
    printf("  Command-Line ATM Interface\n");
    ui_print_line();
}

void ui_print_line(void) {
    printf("--------------------------------------------------\n");
}

void ui_print_error(const char *message) {
    fprintf(stderr, "[ERROR] %s\n", message ? message : "(unknown error)");
}

void ui_print_status(const char *message) {
    printf("[INFO] %s\n", message ? message : "");
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

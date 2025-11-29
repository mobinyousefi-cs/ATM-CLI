/*
 * Project:   Command-Line ATM Interface
 * File:      ui.h
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Terminal user interface utilities (menus, input helpers, formatting).
 */

#ifndef UI_H
#define UI_H

#include "common.h"

/* Visual helpers */
void ui_print_banner(void);
void ui_print_line(void);
void ui_print_error(const char *message);
void ui_print_status(const char *message);

/* Input helpers */
int  ui_read_line(char *buffer, size_t size);
int  ui_read_int(const char *prompt, int *out_value);
int  ui_read_double(const char *prompt, double *out_value);
int  ui_read_string(const char *prompt, char *buffer, size_t size);

#endif /* UI_H */

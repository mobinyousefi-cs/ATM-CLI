/*
 * Project:   Command-Line ATM Interface
 * File:      main.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Entry point for the ATM CLI application.
 *
 *   Usage:
 *     ./atm_cli [accounts_db_file]
 *
 *   If no DB file is provided, "accounts.db" in the current directory is used.
 *   The format is auto-detected:
 *     - *.db or *.csv → CSV format
 *     - *.json        → JSON format
 */

#include "atm.h"
#include "ui.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *default_db = "accounts.db";
    const char *db_path    = default_db;

    if (argc > 1) {
        db_path = argv[1];
    }

    AtmContext ctx;
    AtmStatus st = atm_init(&ctx, db_path);
    if (st != ATM_OK) {
        fprintf(stderr, "Failed to initialize ATM with DB '%s'.\n", db_path);
        return 1;
    }

    atm_run(&ctx);
    atm_shutdown(&ctx);

    return 0;
}

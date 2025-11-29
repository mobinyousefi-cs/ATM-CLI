/*
 * Project:   Command-Line ATM Interface
 * File:      atm.h
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   High-level ATM context and control logic.
 */

#ifndef ATM_H
#define ATM_H

#include "common.h"
#include "account.h"

typedef struct {
    AccountStore store;
    char         db_path[MAX_DB_PATH_LEN];
    int          use_json; /* 0 = CSV, non-zero = JSON */
} AtmContext;

AtmStatus atm_init(AtmContext *ctx, const char *db_path);
void      atm_shutdown(AtmContext *ctx);

/* Main interaction loop (login + per-session menu) */
void      atm_run(AtmContext *ctx);

#endif /* ATM_H */

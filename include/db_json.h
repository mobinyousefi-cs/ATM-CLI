/*
 * Project:   Command-Line ATM Interface
 * File:      db_json.h
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Minimal JSON persistence layer for the account store.
 */

#ifndef DB_JSON_H
#define DB_JSON_H

#include "account.h"
#include "common.h"

AtmStatus account_store_load_json(AccountStore *store, const char *path);
AtmStatus account_store_save_json(const AccountStore *store, const char *path);

#endif /* DB_JSON_H */

/*
 * Project:   Command-Line ATM Interface
 * File:      account.h
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Account data structures and operations for loading, saving,
 *   and manipulating account records.
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "common.h"

typedef struct {
    char     id[MAX_ACCOUNT_ID_LEN];
    char     holder_name[MAX_NAME_LEN];
    double   balance;
    uint32_t pin_hash;
    int      is_locked;        /* 0 = unlocked, non-zero = locked */
    unsigned failed_attempts;  /* consecutive failed PIN attempts */
} Account;

typedef struct {
    Account *items;
    size_t   size;
    size_t   capacity;
} AccountStore;

/* Lifecycle */
AtmStatus account_store_init(AccountStore *store);
void      account_store_free(AccountStore *store);

/* Persistence (CSV) */
AtmStatus account_store_load(AccountStore *store, const char *path);
AtmStatus account_store_save(const AccountStore *store, const char *path);

/* Lookup / manipulation */
Account  *account_store_find(AccountStore *store, const char *account_id);
AtmStatus account_deposit(Account *account, double amount);
AtmStatus account_withdraw(Account *account, double amount);

#endif /* ACCOUNT_H */

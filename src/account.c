/*
 * Project:   Command-Line ATM Interface
 * File:      account.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Implementation of account store management and basic account operations.
 */

#include "account.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static AtmStatus account_store_reserve(AccountStore *store, size_t new_capacity) {
    if (new_capacity <= store->capacity) {
        return ATM_OK;
    }

    Account *new_items = realloc(store->items, new_capacity * sizeof(Account));
    if (!new_items) {
        return ATM_ERR_INTERNAL;
    }

    store->items    = new_items;
    store->capacity = new_capacity;
    return ATM_OK;
}

AtmStatus account_store_init(AccountStore *store) {
    if (!store) return ATM_ERR_INTERNAL;

    store->items    = NULL;
    store->size     = 0;
    store->capacity = 0;

    return ATM_OK;
}

void account_store_free(AccountStore *store) {
    if (!store) return;
    free(store->items);
    store->items    = NULL;
    store->size     = 0;
    store->capacity = 0;
}

Account *account_store_find(AccountStore *store, const char *account_id) {
    if (!store || !account_id) return NULL;

    for (size_t i = 0; i < store->size; ++i) {
        if (strncmp(store->items[i].id, account_id, MAX_ACCOUNT_ID_LEN) == 0) {
            return &store->items[i];
        }
    }
    return NULL;
}

AtmStatus account_store_load(AccountStore *store, const char *path) {
    if (!store || !path) return ATM_ERR_INTERNAL;

    FILE *f = fopen(path, "r");
    if (!f) {
        /* If file does not exist, treat as empty DB */
        return ATM_OK;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), f)) {
        /* Skip empty or commented lines */
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        Account acc;
        memset(&acc, 0, sizeof(Account));

        char id[MAX_ACCOUNT_ID_LEN];
        char name[MAX_NAME_LEN];
        double balance;
        unsigned pin_hash;
        int is_locked;
        unsigned failed_attempts;

        int parsed = sscanf(
            line,
            " %15[^,],%63[^,],%lf,%u,%d,%u",
            id,
            name,
            &balance,
            &pin_hash,
            &is_locked,
            &failed_attempts
        );

        if (parsed != 6) {
            fclose(f);
            return ATM_ERR_PARSE;
        }

        strncpy(acc.id, id, MAX_ACCOUNT_ID_LEN - 1);
        acc.id[MAX_ACCOUNT_ID_LEN - 1] = '\0';

        strncpy(acc.holder_name, name, MAX_NAME_LEN - 1);
        acc.holder_name[MAX_NAME_LEN - 1] = '\0';

        acc.balance         = balance;
        acc.pin_hash        = (uint32_t)pin_hash;
        acc.is_locked       = is_locked;
        acc.failed_attempts = failed_attempts;

        if (store->size == store->capacity) {
            size_t new_cap = (store->capacity == 0) ? 8 : store->capacity * 2;
            AtmStatus st   = account_store_reserve(store, new_cap);
            if (st != ATM_OK) {
                fclose(f);
                return st;
            }
        }

        store->items[store->size++] = acc;
    }

    fclose(f);
    return ATM_OK;
}

AtmStatus account_store_save(const AccountStore *store, const char *path) {
    if (!store || !path) return ATM_ERR_INTERNAL;

    FILE *f = fopen(path, "w");
    if (!f) {
        return ATM_ERR_IO;
    }

    /* Simple CSV-like format:
     * account_id,holder_name,balance,pin_hash,is_locked,failed_attempts
     */
    for (size_t i = 0; i < store->size; ++i) {
        const Account *acc = &store->items[i];
        fprintf(f,
                "%s,%s,%.2f,%u,%d,%u\n",
                acc->id,
                acc->holder_name,
                acc->balance,
                acc->pin_hash,
                acc->is_locked,
                acc->failed_attempts);
    }

    if (fclose(f) != 0) {
        return ATM_ERR_IO;
    }

    return ATM_OK;
}

AtmStatus account_deposit(Account *account, double amount) {
    if (!account) return ATM_ERR_INTERNAL;
    if (amount <= 0.0) return ATM_ERR_INVALID_AMOUNT;

    account->balance += amount;
    return ATM_OK;
}

AtmStatus account_withdraw(Account *account, double amount) {
    if (!account) return ATM_ERR_INTERNAL;
    if (amount <= 0.0) return ATM_ERR_INVALID_AMOUNT;

    if (amount > account->balance) {
        return ATM_ERR_INSUFFICIENT_FUNDS;
    }

    account->balance -= amount;
    return ATM_OK;
}

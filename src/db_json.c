/*
 * Project:   Command-Line ATM Interface
 * File:      db_json.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Minimal JSON persistence layer for the account store.
 *   NOTE: This is a very lightweight, format-specific parser intended
 *         for educational purposes, not a general JSON implementation.
 */

#include "db_json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }

    long size = ftell(f);
    if (size < 0) {
        fclose(f);
        return NULL;
    }
    rewind(f);

    char *buf = malloc((size_t)size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    size_t nread = fread(buf, 1, (size_t)size, f);
    buf[nread] = '\0';

    fclose(f);
    return buf;
}

static void normalize(char *s) {
    if (!s) return;
    char *p = s;
    while (*p) {
        if (*p == '\n' || *p == '\r' || *p == '\t') {
            *p = ' ';
        }
        p++;
    }
}

AtmStatus account_store_load_json(AccountStore *store, const char *path) {
    if (!store || !path) return ATM_ERR_INTERNAL;

    char *text = read_file(path);
    if (!text) {
        /* Treat missing file as empty DB, consistent with CSV loader. */
        return ATM_OK;
    }

    normalize(text);
    char *p = text;

    /* Locate "accounts" keyword */
    p = strstr(p, "\"accounts\"");
    if (!p) {
        free(text);
        return ATM_ERR_PARSE;
    }

    p = strchr(p, '[');
    if (!p) {
        free(text);
        return ATM_ERR_PARSE;
    }
    p++;

    while (*p) {
        /* Skip whitespace and commas */
        while (*p == ' ' || *p == ',' || *p == '\n' || *p == '\r' || *p == '\t') {
            p++;
        }

        if (*p == ']') {
            break;
        }

        Account acc;
        memset(&acc, 0, sizeof(acc));

        /*
         * Expect objects like:
         * {
         *   "id": "1001",
         *   "holder": "John Doe",
         *   "balance": 1500.00,
         *   "pin_hash": 3356862322,
         *   "locked": 0,
         *   "failed": 0
         * }
         */
        int matched = sscanf(
            p,
            " { \"id\" : \"%15[^\"]\" ,"
            " \"holder\" : \"%63[^\"]\" ,"
            " \"balance\" : %lf ,"
            " \"pin_hash\" : %u ,"
            " \"locked\" : %d ,"
            " \"failed\" : %u",
            acc.id,
            acc.holder_name,
            &acc.balance,
            &acc.pin_hash,
            &acc.is_locked,
            &acc.failed_attempts
        );

        if (matched != 6) {
            free(text);
            return ATM_ERR_PARSE;
        }

        /* Grow store if needed */
        if (store->size == store->capacity) {
            size_t new_cap = (store->capacity == 0) ? 8 : store->capacity * 2;
            Account *tmp   = realloc(store->items, new_cap * sizeof(Account));
            if (!tmp) {
                free(text);
                return ATM_ERR_INTERNAL;
            }
            store->items    = tmp;
            store->capacity = new_cap;
        }

        store->items[store->size++] = acc;

        /* Move to next object */
        p = strchr(p, '}');
        if (!p) {
            break;
        }
        p++;
    }

    free(text);
    return ATM_OK;
}

AtmStatus account_store_save_json(const AccountStore *store, const char *path) {
    if (!store || !path) return ATM_ERR_INTERNAL;

    FILE *f = fopen(path, "w");
    if (!f) {
        return ATM_ERR_IO;
    }

    fprintf(f, "{\n  \"accounts\": [\n");

    for (size_t i = 0; i < store->size; ++i) {
        const Account *a = &store->items[i];

        fprintf(f,
                "    {\n"
                "      \"id\": \"%s\",\n"
                "      \"holder\": \"%s\",\n"
                "      \"balance\": %.2f,\n"
                "      \"pin_hash\": %u,\n"
                "      \"locked\": %d,\n"
                "      \"failed\": %u\n"
                "    }%s\n",
                a->id,
                a->holder_name,
                a->balance,
                a->pin_hash,
                a->is_locked,
                a->failed_attempts,
                (i + 1 == store->size) ? "" : ",");
    }

    fprintf(f, "  ]\n}\n");
    fclose(f);

    return ATM_OK;
}

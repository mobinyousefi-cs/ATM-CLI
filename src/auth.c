/*
 * Project:   Command-Line ATM Interface
 * File:      auth.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Implementation of PIN hashing and login verification logic.
 *   Note: This is a demonstration-only, non-cryptographic hash.
 */

#include "auth.h"

#include <string.h>

uint32_t auth_hash_pin(const char *pin) {
    /* FNV-1a style 32-bit hash (not cryptographically secure). */
    const uint32_t FNV_OFFSET = 2166136261u;
    const uint32_t FNV_PRIME  = 16777619u;

    uint32_t hash = FNV_OFFSET;
    const unsigned char *p = (const unsigned char *)pin;

    while (*p) {
        hash ^= (uint32_t)(*p++);
        hash *= FNV_PRIME;
    }
    return hash;
}

AtmStatus auth_verify_login(Account *account, const char *pin) {
    if (!account || !pin) {
        return ATM_ERR_INTERNAL;
    }

    if (account->is_locked) {
        return ATM_ERR_LOCKED;
    }

    uint32_t h = auth_hash_pin(pin);
    if (h == account->pin_hash) {
        account->failed_attempts = 0;
        return ATM_OK;
    }

    /* Wrong PIN */
    account->failed_attempts++;
    if (account->failed_attempts >= MAX_FAILED_ATTEMPTS) {
        account->is_locked = 1;
        return ATM_ERR_LOCKED;
    }

    return ATM_ERR_AUTH_FAILED;
}

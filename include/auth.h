/*
 * Project:   Command-Line ATM Interface
 * File:      auth.h
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Authentication helpers for PIN hashing and login validation.
 */

#ifndef AUTH_H
#define AUTH_H

#include "common.h"
#include "account.h"

/* Non-cryptographic PIN hash for demonstration purposes. */
uint32_t  auth_hash_pin(const char *pin);

/*
 * Verifies a login attempt using the provided PIN.
 * Updates failed_attempts and is_locked fields in the Account when necessary.
 */
AtmStatus auth_verify_login(Account *account, const char *pin);

#endif /* AUTH_H */

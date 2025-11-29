/*
 * Project:   Command-Line ATM Interface
 * File:      common.h
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   Common definitions, constants, and shared types used across the ATM project.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

#define MAX_ACCOUNT_ID_LEN  16
#define MAX_NAME_LEN        64
#define MAX_PIN_LEN         32
#define MAX_LINE_LEN        256
#define MAX_DB_PATH_LEN     260

#define MAX_FAILED_ATTEMPTS 3

typedef enum {
    ATM_OK = 0,
    ATM_ERR_IO,
    ATM_ERR_PARSE,
    ATM_ERR_NOT_FOUND,
    ATM_ERR_AUTH_FAILED,
    ATM_ERR_LOCKED,
    ATM_ERR_INVALID_AMOUNT,
    ATM_ERR_INSUFFICIENT_FUNDS,
    ATM_ERR_INTERNAL
} AtmStatus;

#endif /* COMMON_H */

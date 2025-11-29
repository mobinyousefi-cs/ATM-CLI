/*
 * Project:   Command-Line ATM Interface
 * File:      atm.c
 * Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:   MIT
 *
 * Description:
 *   High-level ATM flow: initialization, login loop, and per-session menu.
 */

#include "atm.h"
#include "auth.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>

static void atm_print_status_from_code(AtmStatus status);
static void atm_session(AtmContext *ctx, Account *account);

AtmStatus atm_init(AtmContext *ctx, const char *db_path) {
    if (!ctx || !db_path) return ATM_ERR_INTERNAL;

    AtmStatus st = account_store_init(&ctx->store);
    if (st != ATM_OK) {
        return st;
    }

    strncpy(ctx->db_path, db_path, MAX_DB_PATH_LEN - 1);
    ctx->db_path[MAX_DB_PATH_LEN - 1] = '\0';

    st = account_store_load(&ctx->store, ctx->db_path);
    if (st != ATM_OK) {
        return st;
    }

    return ATM_OK;
}

void atm_shutdown(AtmContext *ctx) {
    if (!ctx) return;
    account_store_free(&ctx->store);
}

void atm_run(AtmContext *ctx) {
    if (!ctx) return;

    ui_print_banner();
    printf("Database file: %s\n", ctx->db_path);
    ui_print_line();

    char account_id[MAX_ACCOUNT_ID_LEN];
    char pin[MAX_PIN_LEN];

    for (;;) {
        printf("\nType 'q' to exit the ATM.\n");
        if (!ui_read_string("Enter account ID: ", account_id, sizeof(account_id))) {
            ui_print_error("Failed to read account ID.");
            break;
        }

        if (strcmp(account_id, "q") == 0 || strcmp(account_id, "Q") == 0) {
            ui_print_status("Exiting ATM. Goodbye.");
            break;
        }

        Account *acc = account_store_find(&ctx->store, account_id);
        if (!acc) {
            ui_print_error("Account not found.");
            continue;
        }

        if (acc->is_locked) {
            ui_print_error("Account is locked due to too many failed attempts. Please contact the bank.");
            continue;
        }

        if (!ui_read_string("Enter PIN: ", pin, sizeof(pin))) {
            ui_print_error("Failed to read PIN.");
            continue;
        }

        AtmStatus auth_status = auth_verify_login(acc, pin);
        if (auth_status == ATM_OK) {
            ui_print_status("Authentication successful. Welcome!");
            /* Persist auth-related state (failed_attempts reset, etc.) */
            AtmStatus st = account_store_save(&ctx->store, ctx->db_path);
            if (st != ATM_OK) {
                atm_print_status_from_code(st);
            }
            atm_session(ctx, acc);
        } else {
            atm_print_status_from_code(auth_status);
            /* Save in case account becomes locked */
            AtmStatus st = account_store_save(&ctx->store, ctx->db_path);
            if (st != ATM_OK) {
                atm_print_status_from_code(st);
            }
        }
    }
}

static void atm_session(AtmContext *ctx, Account *account) {
    if (!ctx || !account) return;

    int choice = 0;
    double amount = 0.0;

    for (;;) {
        ui_print_line();
        printf("Account ID: %s\n", account->id);
        printf("Account Holder: %s\n", account->holder_name);
        ui_print_line();
        printf("1) Balance inquiry\n");
        printf("2) Deposit\n");
        printf("3) Withdraw\n");
        printf("4) Logout\n");

        if (!ui_read_int("Select an option: ", &choice)) {
            ui_print_error("Failed to read menu option.");
            continue;
        }

        switch (choice) {
        case 1:
            printf("Current balance: %.2f\n", account->balance);
            break;

        case 2:
            if (!ui_read_double("Enter deposit amount: ", &amount)) {
                ui_print_error("Failed to read amount.");
                break;
            }
            switch (account_deposit(account, amount)) {
            case ATM_OK:
                ui_print_status("Deposit successful.");
                break;
            case ATM_ERR_INVALID_AMOUNT:
                ui_print_error("Invalid deposit amount.");
                break;
            default:
                ui_print_error("Unexpected error during deposit.");
                break;
            }
            /* Persist balance */
            atm_print_status_from_code(account_store_save(&ctx->store, ctx->db_path));
            break;

        case 3:
            if (!ui_read_double("Enter withdrawal amount: ", &amount)) {
                ui_print_error("Failed to read amount.");
                break;
            }
            switch (account_withdraw(account, amount)) {
            case ATM_OK:
                ui_print_status("Withdrawal successful.");
                break;
            case ATM_ERR_INVALID_AMOUNT:
                ui_print_error("Invalid withdrawal amount.");
                break;
            case ATM_ERR_INSUFFICIENT_FUNDS:
                ui_print_error("Insufficient funds.");
                break;
            default:
                ui_print_error("Unexpected error during withdrawal.");
                break;
            }
            /* Persist balance */
            atm_print_status_from_code(account_store_save(&ctx->store, ctx->db_path));
            break;

        case 4:
            ui_print_status("Logging out...");
            return;

        default:
            ui_print_error("Unknown menu option. Please try again.");
            break;
        }
    }
}

static void atm_print_status_from_code(AtmStatus status) {
    switch (status) {
    case ATM_OK:
        /* No message needed */
        break;
    case ATM_ERR_IO:
        ui_print_error("I/O error while accessing the account database.");
        break;
    case ATM_ERR_PARSE:
        ui_print_error("Failed to parse account database. Check file format.");
        break;
    case ATM_ERR_NOT_FOUND:
        ui_print_error("Requested resource not found.");
        break;
    case ATM_ERR_AUTH_FAILED:
        ui_print_error("Authentication failed. Incorrect PIN.");
        break;
    case ATM_ERR_LOCKED:
        ui_print_error("Account locked due to multiple failed attempts.");
        break;
    case ATM_ERR_INVALID_AMOUNT:
        ui_print_error("Invalid transaction amount.");
        break;
    case ATM_ERR_INSUFFICIENT_FUNDS:
        ui_print_error("Insufficient funds for transaction.");
        break;
    case ATM_ERR_INTERNAL:
    default:
        ui_print_error("Internal error occurred.");
        break;
    }
}

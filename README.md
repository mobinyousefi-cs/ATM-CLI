# Command-Line ATM Interface

A Fully Modular C Project by **Mobin Yousefi**

          ___    ________  ___      ___  ________  _________   
         |\  \  |\   __  \|\  \    /  /||\   __  \|\___   ___\ 
         \ \  \ \ \  \|\  \ \  \  /  / /\ \  \|\  \|___ \  \_| 
       __ \ \  \ \ \   __  \ \  \/  / /  \ \   __  \   \ \  \  
      |\  \\_\  \ \ \  \ \  \ \    / /    \ \  \ \  \   \ \  \ 
      \ \________\ \ \__\ \__\ \__/ /      \ \__\ \__\   \ \__\
       \|________|\|__|\|__|\|__|/        \|__|\|__|    \|__|

## Status & Metadata

[![C Language](https://img.shields.io/badge/language-C-blue.svg)]()\
[![Build](https://img.shields.io/badge/build-Makefile-success)]()\
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)]()\
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)]()\
[![Author](https://img.shields.io/badge/Author-Mobin%20Yousefi-black)](https://github.com/mobinyousefi-cs)

## Overview

The **Command-Line ATM Interface** is a fully modular and professionally
structured C program that simulates essential ATM functionalities:
authentication, balance inquiry, deposit/withdrawal, and persistent
account storage.

## Build

    make

## Usage

    ./atm_cli

To use a custom DB:

    ./atm_cli ./data/my_accounts.db

## Database Format

    account_id,holder_name,balance,pin_hash,is_locked,failed_attempts
    1001,John Doe,1500.00,3356862322,0,0

## License

MIT License.

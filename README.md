# Restaurant Permissions Project

## Overview

This C language project is designed to explore the use of bitwise operators and bit flags for efficient handling of permissions in a restaurant management system. The primary aim is to understand how bitwise operations can be leveraged to manage user permissions using compact bit flags.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Bitwise Operators and Permissions](#bitwise-operators-and-permissions)
- [Roles and Permissions](#roles-and-permissions)

## Installation

To get started with the project, follow these installation steps [provide instructions if any].

## Usage

This project serves as a learning ground for bitwise operations in the context of a restaurant management system. Explore the code to understand how permissions are managed and how different roles are assigned using bitwise flags.

## Bitwise Operators and Permissions

Bitwise operators, such as AND (`&`), OR (`|`), and left shift (`<<`), are fundamental to the project. These operators are used to manipulate individual bits in an integer, representing various permissions. The compact nature of bitwise flags allows for efficient storage and retrieval of permission information.

## Roles and Permissions

Roles, such as the restaurant owner and waiter, are defined with specific permission sets. Each permission is represented by a unique bit, and roles are created by combining these permissions using bitwise OR (`|`). Checking for permissions involves using the bitwise AND (`&`) operator. This approach provides a concise way to manage and query permissions.

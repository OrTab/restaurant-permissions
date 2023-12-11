# Restaurant Permissions Project

## Overview

Welcome to the Restaurant Permissions Project! This C language project provides practical experience with bitwise operators, memory management, string handling, and file I/O in the context of a restaurant management system. The primary focus is on bitwise operations, enabling efficient user permission management through compact bit flags.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Key Aspects](#key-aspects)
  - [Bitwise Operators](#bitwise-operators)
  - [Memory Management](#memory-management)
  - [String Handling](#string-handling)
  - [File I/O](#file-io)
  - [Header Files](#header-files)

## Installation

To start with the project:

1. Ensure you have a C compiler installed (you can use any C compiler). Check by running `gcc -v`.
2. Compile the project using `gcc main.c restaurant_permissions.c -o a.out`.
3. Execute the generated output file.

## Usage

Explore the code to understand the implementation of bitwise operations, memory management, string handling, and file I/O within a restaurant management system. The project provides hands-on experience and practical knowledge in these essential areas of C programming.

## Key Aspects

### Bitwise Operators

Gain insights into fundamental bitwise operators such as AND (`&`), OR (`|`), and left shift (`<<`). Understand their role in manipulating individual bits, representing various permissions. The project demonstrates efficient handling of permission information.

### Memory Management

Dive into memory management practices, including dynamic memory allocation and deallocation. Learn to use `malloc` and `free` for efficient handling of user names and structs, preventing memory leaks.

### String Handling

Explore robust string handling techniques, emphasizing the use of functions like `strcpy` for managing and manipulating user names. In this project, dynamic memory allocation is employed when reading user names from a file, considering the string length property. Similarly, when adding a new user, memory is dynamically allocated to store the user's name. This ensures efficient memory usage and adaptability to varying string lengths in the C language.

Learn effective ways to work with strings, incorporating dynamic memory allocation for user names when reading from files and when adding new users.

### File I/O

Understand file input and output operations for reading from and writing to files. Learn how to store user data persistently, enabling information retrieval between program executions.

### Header Files

This project makes use of modular programming by employing header files to organize and encapsulate related functionality. Various aspects of the project, such as user management, permissions, and roles, are encapsulated in separate header files (`headers folder`). These header files serve as helpers, providing clear interfaces and aiding in code organization.

Explore the contents of these header files to understand how they facilitate modularity, encapsulation, and code reuse. Header files play a crucial role in making the codebase more maintainable and understandable, promoting a modular design approach.

Happy coding!

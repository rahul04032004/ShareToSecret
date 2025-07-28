# Shamir's Secret Sharing (C++)

## Overview
This project solves a simplified Shamir's Secret Sharing problem using Lagrange Interpolation, as per the Catalog Placements Assignment instructions. It reads input from JSON files, decodes values encoded in different bases, and computes the constant term of the polynomial (the secret).

## Features
- **C++ only**: No Python or external scripting used
- **Reads input from JSON files** (see `input/` directory)
- **Decodes y values** from arbitrary bases
- **Finds the constant term (secret)** using Lagrange interpolation
- **Handles large numbers (within 64-bit integer range)**

## Assignment Checkpoints
- Reads test cases from separate JSON files
- Decodes y values from their specified bases
- Finds the secret (constant term) for each test case
- Prints the secret for each test case, one per line

## Input Format
Each test case is a JSON file in the `input/` directory, e.g.:
```json
{
    "keys": {
        "n": 4,
        "k": 3
    },
    "1": { "base": "10", "value": "4" },
    "2": { "base": "2", "value": "111" },
    "3": { "base": "10", "value": "12" },
    "4": { "base": "4", "value": "213" }
}
```

## Build Instructions
You need a C++17 compiler (e.g., g++). On Linux/WSL:

```bash
# From the project root
make
```

Or manually:
```bash
# From the project root
wsl g++ -std=c++17 -Wall -Iinclude -o secret_share src/main.cpp src/shamir.cpp
```

## Run Instructions
```bash
wsl ./secret_share
```

## Sample Output
```
3
-355018772209779925
```
Each line is the secret for one test case, in order.

## Notes
- No Python code is present or required.
- Only the two required test cases are included and processed.
- Output matches the assignment requirements exactly.

## Submission
Push this code to GitHub and submit the link along with the output above.

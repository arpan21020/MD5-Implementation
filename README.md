# MD5 Hash Implementation in C++

This repository contains a C++ implementation of the MD5 (Message Digest Algorithm 5) hashing algorithm. MD5 is a widely used cryptographic hash function that produces a 128-bit (16-byte) hash value, typically expressed as a 32-character hexadecimal number.

## Table of Contents
- [Overview](#overview)
- [Algorithm Details](#algorithm-details)
- [Requirements](#requirements)
- [Example](#example)
- [Implementation Details](#implementation-details)
- [Security Considerations](#security-considerations)
- [License](#license)

## Overview

MD5 was designed by Ronald Rivest in 1991 to replace an earlier hash function, MD4. Although MD5 is no longer considered secure against well-funded attackers, it remains widely used for non-cryptographic purposes such as checksums to verify data integrity and detect unintentional data corruption.

This implementation follows the standard MD5 algorithm as described in [RFC 1321](https://tools.ietf.org/html/rfc1321).

## Algorithm Details

The MD5 algorithm works as follows:

1. **Padding**: The message is padded so that its length in bits is congruent to 448, modulo 512.
2. **Append Length**: A 64-bit representation of the original message length is appended.
3. **Initialize Buffers**: Four 32-bit words (A, B, C, D) are initialized with specific constants.
4. **Process Message**: The message is processed in 512-bit blocks through four rounds of operations.
5. **Output**: The final output is the concatenation of the four 32-bit words.

## Requirements

- C++ compiler with C++11 support
- Standard Template Library (STL)


## Example

The provided `main()` function demonstrates basic usage by:
1. Creating an MD5Hash object
2. Accepting user input from the console
3. Computing the hash of the input string
4. Displaying the resulting hash value

Sample execution:
```
Enter a string to hash: abc
MD5 hash: 900150983cd24fb0d6963f7d28e17f72
```

## Implementation Details

The implementation consists of a single class `MD5Hash` with the following components:

### Private Members:
- **Constants**: Initial word values and pre-computed tables
- **Helper Functions**: F, G, H, I auxiliary functions as defined in the MD5 specification
- **Processing Functions**: Methods for padding messages, processing blocks, and formatting the output

### Public Interface:
- **Constructor**: Initializes the hash state
- **hashing**: Public method to compute an MD5 hash from an input string

### Key Functions:
1. **string_to_bytes**: Converts input string to a byte array
2. **pad_message**: Applies the required padding to meet the algorithm's specifications
3. **process_block**: Processes each 512-bit block of the message
4. **digest_to_hex**: Converts the final state to a hexadecimal string

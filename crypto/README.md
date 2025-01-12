holbertonschool-blockchain
├── blockchain
│   └── v0.1
│       ├── blockchain_create.c
│       ├── blockchain_deserialize.c
│       ├── blockchain_destroy.c
│       ├── blockchain.h
│       ├── blockchain_serialize.c
│       ├── block_create.c
│       ├── block_destroy.c
│       ├── block_hash.c
│       ├── block_is_valid.c
│       ├── libhblk_blockchain_v0.1.a
│       ├── Makefile
│       ├── provided
│       │   ├── _blockchain_destroy.c
│       │   ├── _blockchain_print.c
│       │   ├── _endianness.c
│       │   ├── endianness.h
│       │   └── _genesis.c
│       └── test
│           ├── blockchain_create-main.c
│           ├── blockchain_deserialize-main.c
│           ├── blockchain_destroy-main.c
│           ├── blockchain_serialize-main.c
│           ├── block_create-main.c
│           ├── block_destroy-main.c
│           ├── block_hash-main.c
│           └── block_is_valid-main.c
├── crypto/
└── README.md
Tasks
0. Create Blockchain
mandatory
Write a function that creates a Blockchain structure, and initializes it.

Prototype: blockchain_t *blockchain_create(void);
The Blockchain must contain one block upon creation. This block is called the Genesis Block. Its content is static, and pre-defined as follows:
index -> 0
difficulty -> 0
timestamp -> 1537578000
nonce -> 0
prev_hash -> 0 (times 32)
data -> "Holberton School" (16 bytes long)
hash -> c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
Note: The above is the hexadecimal representation of the hash, you can store it in the hash variable as follows:
hash = "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" /* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */
Example:

alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/blockchain_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <llist.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    blockchain_t *blockchain2;

    blockchain = blockchain_create();

    _blockchain_print(blockchain);

    blockchain2 = blockchain_create();
    if (memcmp(llist_get_head(blockchain->chain),
        llist_get_head(blockchain2->chain), sizeof(block_t)) != 0)
    {
        fprintf(stderr, "Genesis Block should always be the same\n");
        _blockchain_destroy(blockchain);
        _blockchain_destroy(blockchain2);
        return (EXIT_FAILURE);
    }

    _blockchain_destroy(blockchain);
    _blockchain_destroy(blockchain2);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o blockchain_create-test blockchain_create.c test/blockchain_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./blockchain_create-test
Blockchain: {
    chain [1]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$
Repo:

GitHub repository: alu-blockchain
File: blockchain/v0.1/blockchain_create.c
0/3 pts
1. Create Block
mandatory
Write a function that creates a Block structure and initializes it.

Prototype: block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len), where:
prev is a pointer to the previous Block in the Blockchain
data points to a memory area to duplicate in the Block’s data
data_len stores the number of bytes to duplicate in data. If data_len is bigger than BLOCKCHAIN_DATA_MAX, then only BLOCKCHAIN_DATA_MAX bytes must be duplicated.
The new Block’s index must be the previous Block’s index + 1
The new Block’s difficulty and nonce must both be initialized to 0
The new Block’s timestamp should be initialized using the time(2) system call. It is normal if your value differs from the following example.
The new Block’s hash must be zeroed
Your function must return a pointer to the allocated Block
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;

    blockchain = blockchain_create();
    block = llist_get_head(blockchain->chain);

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"School", 6);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    _blockchain_print(blockchain);
    _blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_create-test blockchain_create.c block_create.c test/block_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./block_create-test
Blockchain: {
    chain [3]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: {
                index: 1,
                difficulty: 0,
                timestamp: 1536714911,
                nonce: 0,
                prev_hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
            },
            data: {
                buffer: "Holberton",
                len: 9
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
        Block: {
            info: {
                index: 2,
                difficulty: 0,
                timestamp: 1536714911,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "School",
                len: 6
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$
Repo:

GitHub repository: alu-blockchain
File: blockchain/v0.1/block_create.c
0/4 pts
2. Delete Block
mandatory
Write a function that deletes an existing Block

Prototype: void block_destroy(block_t *block);, where:
block points to the Block to delete
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_destroy-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *first, *block1, *block2;

    blockchain = blockchain_create();
    first = llist_pop(blockchain->chain);
    block1 = block_create(first, (int8_t *)"Holberton", 9);
    block2 = block_create(block1, (int8_t *)"School", 6);

    block_destroy(first);
    block_destroy(block1);
    block_destroy(block2);

    _blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_destroy-test blockchain_create.c block_create.c block_destroy.c test/block_destroy-main.c provided/_blockchain_destroy.c  -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ valgrind ./block_destroy-test
==3393== Memcheck, a memory error detector
==3393== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==3393== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==3393== Command: ./block_destroy-test
==3393==
==3393==
==3393== HEAP SUMMARY:
==3393==     in use at exit: 0 bytes in 0 blocks
==3393==   total heap usage: 6 allocs, 6 frees, 3,480 bytes allocated
==3393==
==3393== All heap blocks were freed -- no leaks are possible
==3393==
==3393== For counts of detected and suppressed errors, rerun with: -v
==3393== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/blockchain/v0.1$
Repo:

GitHub repository: alu-blockchain
File: blockchain/v0.1/block_destroy.c
0/4 pts
3. Delete Blockchain
mandatory
Write a function that deletes an existing Blockchain, along with all the Blocks it contains

Prototype: void blockchain_destroy(blockchain_t *blockchain);, where:
blockchain points to the Blockchain structure to delete
All the Blocks in the chain linked list must be destroyed
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/blockchain_destroy-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;

    blockchain = blockchain_create();
    block = llist_get_head(blockchain->chain);

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"School", 6);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"of", 2);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"Software", 8);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"Engineering", 11);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o blockchain_destroy-test blockchain_destroy.c blockchain_create.c block_create.c block_destroy.c test/blockchain_destroy-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ valgrind ./blockchain_destroy-test
==3405== Memcheck, a memory error detector
==3405== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==3405== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==3405== Command: ./blockchain_destroy-test
==3405==
==3405==
==3405== HEAP SUMMARY:
==3405==     in use at exit: 0 bytes in 0 blocks
==3405==   total heap usage: 14 allocs, 14 frees, 6,920 bytes allocated
==3405==
==3405== All heap blocks were freed -- no leaks are possible
==3405==
==3405== For counts of detected and suppressed errors, rerun with: -v
==3405== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/blockchain/v0.1$
Repo:

GitHub repository: alu-blockchain
File: blockchain/v0.1/blockchain_destroy.c
0/6 pts
4. Hash Block
mandatory
Write a function that computes the hash of a Block

Prototype: uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);, where:
block points to the Block to be hashed
The resulting hash must be stored in hash_buf
block->hash must be left unchanged.
Your function must return a pointer to hash_buf
You don’t have to allocate any memory dynamically
Note: With this first iteration of our Block structure, the only data that is unpredictable is the timestamp. Feel free to hardcode the timestamp from the following example to validate your output.

alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_hash-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;

    blockchain = blockchain_create();
    block = llist_get_head(blockchain->chain);

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    _blockchain_print(blockchain);

    block_hash(block, block->hash);
    block = block_create(block, (int8_t *)"School", 6);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    _blockchain_print(blockchain);

    _blockchain_destroy(blockchain);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_hash-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c test/block_hash-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./block_hash-test
Blockchain: {
    chain [2]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: {
                index: 1,
                difficulty: 0,
                timestamp: 1536715352,
                nonce: 0,
                prev_hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
            },
            data: {
                buffer: "Holberton",
                len: 9
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
    ]
}
Blockchain: {
    chain [3]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: {
                index: 1,
                difficulty: 0,
                timestamp: 1536715352,
                nonce: 0,
                prev_hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
            },
            data: {
                buffer: "Holberton",
                len: 9
            },
            hash: ddcfaf37a7509f99f698bc055885b222e690efe26a74a71eed452dc0f0808d46
        }
        Block: {
            info: {
                index: 2,
                difficulty: 0,
                timestamp: 1536715352,

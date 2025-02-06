#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"
#include "transaction.h"
#include "crypto.h"

#define MAX_TRANSACTIONS 100 // Maximum transactions per block
#define MINING_DIFFICULTY 4   // Example difficulty level

/**
 * Perform Proof of Work by finding a valid hash below a given difficulty target.
 */
void proof_of_work(Block *block) {
    char hash[SHA256_DIGEST_LENGTH * 2 + 1]; // Store hex hash
    int nonce = 0;

    do {
        block->nonce = nonce;
        compute_block_hash(block, hash);
        nonce++;
    } while (strncmp(hash, "0000", MINING_DIFFICULTY) != 0); // Adjust difficulty

    printf("Block mined! Hash: %s\n", hash);
}

/**
 * Mine a new block and add it to the blockchain.
 */
int mine_block(Blockchain *chain, TransactionPool *tx_pool, UTXOSet *utxo_set) {
    if (tx_pool->count == 0) {
        printf("No transactions to mine.\n");
        return 0;
    }

    Block *new_block = create_new_block(chain->last_block);
    if (!new_block) {
        fprintf(stderr, "Error creating new block.\n");
        return 0;
    }

    // Add coinbase transaction (reward)
    Transaction coinbase_tx = create_coinbase_transaction(chain->length + 1);
    add_transaction_to_block(new_block, &coinbase_tx);
    update_utxo_set(utxo_set, &coinbase_tx);

    // Add valid transactions from the pool
    for (int i = 0; i < tx_pool->count; i++) {
        if (validate_transaction(&tx_pool->transactions[i], utxo_set)) {
            add_transaction_to_block(new_block, &tx_pool->transactions[i]);
            update_utxo_set(utxo_set, &tx_pool->transactions[i]);
        } else {
            printf("Invalid transaction removed from pool.\n");
        }
    }

    // Perform Proof of Work
    proof_of_work(new_block);

    // Validate and add block to the chain
    if (validate_block(new_block, chain)) {
        add_block_to_chain(chain, new_block);
        printf("Block %d added to the blockchain.\n", chain->length);
        return 1;
    } else {
        printf("Block validation failed.\n");
        return 0;
    }
}

int main() {
    Blockchain chain = load_blockchain();
    TransactionPool tx_pool = load_transaction_pool();
    UTXOSet utxo_set = load_utxo_set();

    if (mine_block(&chain, &tx_pool, &utxo_set)) {
        save_blockchain(&chain);
        save_utxo_set(&utxo_set);
    }

    return 0;
}


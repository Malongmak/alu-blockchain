#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"
#include "transaction.h"

/**
 * Display blockchain statistics.
 */
void display_blockchain_info() {
    // Load blockchain data
    Blockchain chain = load_blockchain();
    TransactionPool tx_pool = load_transaction_pool();
    UTXOSet utxo_set = load_utxo_set();

    // Display information
    printf("Blockchain Information:\n");
    printf("------------------------\n");
    printf("Number of Blocks: %d\n", chain.length);
    printf("Number of Unspent Transaction Outputs (UTXOs): %d\n", utxo_set.count);
    printf("Number of Pending Transactions: %d\n", tx_pool.count);
}

int main() {
    display_blockchain_info();
    return 0;
}


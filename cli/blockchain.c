#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * Serialize blockchain to a file.
 */
int serialize_blockchain(Blockchain *chain, FILE *file) {
    if (!fwrite(chain, sizeof(Blockchain), 1, file)) {
        return 0;
    }
    return 1;
}

/**
 * Deserialize blockchain from a file.
 */
Blockchain* deserialize_blockchain(FILE *file) {
    Blockchain *chain = malloc(sizeof(Blockchain));
    if (!chain) {
        return NULL;
    }

    if (!fread(chain, sizeof(Blockchain), 1, file)) {
        free(chain);
        return NULL;
    }

    return chain;
}


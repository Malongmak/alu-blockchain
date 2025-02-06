#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * Load a blockchain from a file and override the local blockchain.
 */
int load_blockchain_from_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Error opening blockchain file");
        return 0;
    }

    Blockchain *chain = deserialize_blockchain(file);
    fclose(file);

    if (!chain) {
        fprintf(stderr, "Failed to load blockchain from file.\n");
        return 0;
    }

    save_blockchain(chain);  // Override the current blockchain
    printf("Blockchain successfully loaded from %s\n", path);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: load <path>\n");
        return 1;
    }

    if (load_blockchain_from_file(argv[1])) {
        printf("Run './info' to verify the blockchain has been loaded.\n");
    }

    return 0;
}


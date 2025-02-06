#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * Save the blockchain to a file.
 */
int save_blockchain_to_file(const char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("Error opening file for saving");
        return 0;
    }

    Blockchain *chain = load_blockchain();
    if (!chain) {
        fprintf(stderr, "Error loading local blockchain.\n");
        fclose(file);
        return 0;
    }

    if (!serialize_blockchain(chain, file)) {
        fprintf(stderr, "Error saving blockchain to file.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    printf("Blockchain successfully saved to %s\n", path);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: save <path>\n");
        return 1;
    }

    save_blockchain_to_file(argv[1]);
    return 0;
}


// JIVAJ BRAR dehuff.c
#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STACK_SIZE 64
struct Node *stack[STACK_SIZE];

#define HELPMESSAGE                                                                                \
    "Usage: dehuff -i infile -o outfile\n       dehuff -v -i infile -o outfile\n       dehuff "    \
    "-h\n"

int stacktop = -1;

void stackpush(struct Node *node) {
    if (stacktop >= STACK_SIZE - 1) {
        fprintf(stderr, "stack is full\n");
        exit(EXIT_FAILURE);
    }
    stack[++stacktop] = node;
}

struct Node *stackpop(void) {
    if (stacktop < 0) {
        fprintf(stderr, "stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack[stacktop--];
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t leaves = bit_read_uint16(inbuf);
    assert(type1 == 'H'), assert(type2 == 'C');
    int nodes = (leaves * 2) - 1;
    struct Node *newnode;

    for (int j = 0; nodes > j; j++) {

        uint8_t bit = bit_read_bit(inbuf);

        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            newnode = node_create(symbol, 0);
        } else {
            newnode = node_create(0, 0);
            newnode->right = stackpop();
            newnode->left = stackpop();
        }

        stackpush(newnode);
    }
    struct Node *code_tree = stackpop();
    for (uint32_t j = 0; filesize > j; j++) {
        newnode = code_tree;
        while (1) {
            uint8_t bit = bit_read_bit(inbuf);
            if (bit != 0) {
                newnode = newnode->right;
            } else {
                newnode = newnode->left;
            }
            if (newnode->right == NULL && newnode->left == NULL)
                break;
        }
        fputc(newnode->symbol, fout);
    }
    node_free(&code_tree);
}

int main(int argc, char **argv) {
    FILE *output_file = NULL;
    BitReader *input_file = NULL;
    char options[] = "i:o:h";
    int userinp;

    while (-1 != (userinp = getopt(argc, argv, options))) {
        switch (userinp) {
        case 'i':
            input_file = bit_read_open(optarg);
            if (NULL == input_file) {
                fprintf(stderr, "dehuff:  error reading input file %s\n", optarg);
                fprintf(stderr, HELPMESSAGE);
                exit(1);
            } else {
                break;
            }
        case 'o':
            output_file = fopen(optarg, "wb");
            if (NULL == output_file) {
                printf("dehuff:  unknown or poorly formatted option -o\n");
                exit(1);
            }
            break;
        case 'h': fprintf(stderr, HELPMESSAGE); exit(0);
        }
    }
    if (input_file == NULL) {
        fprintf(stderr, "dehuff:  -i option is required\n");
        fprintf(stderr, HELPMESSAGE);
        exit(1);
    } else if (output_file == NULL) {
        fprintf(stderr, "dehuff:  -o option is required\n");
        fprintf(stderr, HELPMESSAGE);
        exit(1);
    }
    dehuff_decompress_file(output_file, input_file);

    fclose(output_file), bit_read_close(&input_file);

    return 0;
}

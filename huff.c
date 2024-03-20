// JIVAJ BRAR huff.c
#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

#define HELPMESSAGE                                                                                \
    "Usage: huff -i infile -o outfile\n       huff -v -i infile -o outfile\n       huff -h\n"

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    if (histogram == NULL)
        exit(0);
    int tempval;
    uint32_t file = 0;
    //ensures histogram values are all 0
    for (int j = 0; 256 > j; j++)
        histogram[j] = 0;
    histogram[0x00]++, histogram[0xFF]++;

    //update vals
    while ((tempval = fgetc(fin)) != EOF)
        histogram[tempval]++, file++;

    return file;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    if (num_leaves == NULL || histogram == NULL)
        return NULL;
    *num_leaves = 0;

    PriorityQueue *pqueue = pq_create();
    if (pqueue == NULL)
        return NULL;

    int j = 0;
    while (j < 256) {
        if (histogram[j] != 0) {
            Node *newnode = node_create((uint8_t) j, histogram[j]);
            if (newnode == NULL)
                pq_free(&pqueue);
            enqueue(pqueue, newnode);
            (*num_leaves)++;
        }
        j++;
    }
    while (pq_size_is_1(pqueue) == false && pq_is_empty(pqueue) == false) {
        Node *left = dequeue(pqueue), *right = dequeue(pqueue),
             *newnode = node_create(0, left->weight + right->weight);
        if (newnode == NULL) {
            pq_free(&pqueue);
            return NULL;
        }

        newnode->left = left, newnode->right = right;
        enqueue(pqueue, newnode);
    }
    Node *tree = dequeue(pqueue);
    pq_free(&pqueue);
    return tree;
}
void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left != NULL) {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    } else {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    }
}
void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node->left == NULL || node->right == NULL) {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    } else {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= (uint64_t) 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H'), bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);
    while (1) {
        int k = fgetc(fin);
        if (k == EOF)
            break;
        uint64_t code = code_table[k].code;
        uint8_t code_length = code_table[k].code_length;

        for (int j = 0; code_length > j; j++)
            bit_write_bit(outbuf, code & 1), code >>= 1;
    }
}
int main(int argc, char **argv) {
    FILE *input_file = NULL;
    BitWriter *output_file = NULL;

    char options[] = "i:o:h";
    int userinp;
    while (-1 != (userinp = getopt(argc, argv, options))) {
        switch (userinp) {
        case 'i':
            input_file = fopen(optarg, "rb");
            if (NULL == input_file) {
                fprintf(stderr, "huff:  error reading input file %s\n", optarg);
                fprintf(stderr, HELPMESSAGE);
                exit(1);
            } else {
                break;
            }
        case 'o':
            output_file = bit_write_open(optarg);
            if (NULL == output_file) {
                printf("huff:  unknown or poorly formatted option -o\n");
                exit(1);
            }
            break;
        case 'h': fprintf(stderr, HELPMESSAGE); exit(0);
        }
    }
    if (input_file == NULL) {
        fprintf(stderr, "huff:  -i option is required\n");
        fprintf(stderr, HELPMESSAGE);
        exit(1);
    } else if (output_file == NULL) {
        fprintf(stderr, "huff:  -o option is required\n");
        fprintf(stderr, HELPMESSAGE);
        exit(1);
    }
    //initial variables
    uint16_t leaves = 0;
    uint32_t *hist = calloc(256, sizeof(uint32_t)), file = fill_histogram(input_file, hist);

    //initialize NODE and CODE
    Node *tree = create_tree(hist, &leaves);
    Code *code_table = calloc(256, sizeof(Code));
    fill_code_table(code_table, tree, 0, 0);

    //moves to top
    fseek(input_file, 0, SEEK_SET);

    huff_compress_file(output_file, input_file, file, leaves, tree, code_table);

    //frees and closes variables
    free(hist), free(code_table), node_free(&tree);
    bit_write_close(&output_file), fclose(input_file);

    return 0;
}

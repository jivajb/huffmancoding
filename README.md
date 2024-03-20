# Huffman Coding

## Purpose:
The purpose of the Huffman coding program is to input a file and return an encoded version of the file, and also take in a encoded file and return the decoded text file.

## How to use the program:
The program would be used by opening the terminal, and navigating to the directory where all the files are stored, and typing make. These steps create the executable huff and dehuff, that you can input text files into ./huff <-i input file -o output file -h help message> and ./dehuff <-i input file -o output file -h help message>. The input file for huff will be normal text file and the output file will be an encoded version of the text. The input for the dehuff will be an encoded file and the return will be a normal text file. The makefile will also create brtest, bwtest, nodetest, and pqtest, and all of these test the functionality of the helper files bitreader.c, bitwriter.c, node.c, and pq.c.

## Design: 
The program will read the text file with fopen. Then it will create a tree with nodes, and the each node will represent a part of the data and substituting the nodes for that specific part of the data, which would be the tree as a whole, represents the entirety of the given data. This all returns the encoded data. The Huffman decoder will take the encoded data and decode it and give you back the original text.

## Functions: 
- BitWriter *bit_write_open(const char *filename): Takes in a file, and fopens a binary file, and then return all the values but if any of the values don’t return properly return NULL.
- void bit_write_close(BitWriter **pbuf): takes in a pointer given by bitwriter bit_write open. Flushes data from buffer, and closes the underlying stream, and sets the pointer to NULL. if anything goes wrong return NULL.
- void bit_write_bit(BitWriter *buf, uint8_t bit): takes in a bitwriter pointer, and a bit. And it writes puts 8 bits into a buffer byte and the fputc() them. If there are any fatal error, it reports an error.
- void bit_write_uint8(BitWriter *buf, uint8_t x): takes in a bitwriter pointer and a bit, and calls bit_write_bit 8 times. The writing starts with the LSB (least significant or righthand bit).
- void bit_write_uint16(BitWriter *buf, uint16_t x): takes in a bitwriter pointer and a bit, and calls bit_write_bit 16 times. The writing starts with the LSB (least significant or righthand bit).
- void bit_write_uint32(BitWriter *buf, uint32_t x): takes in a bitwriter pointer and a bit, and calls bit_write_bit 32 times. The writing starts with the LSB (least significant or righthand bit).
- BitReader *bit_read_open(const char *filename): takes in a file, fopen() the binary file. Then uses bit read bit to read the first bit. If there are an error, returns fatal error.
- void bit_read_close(BitReader **pbuf): takes in a bitreader double pointer, and closes the *pbuf->underlying_stream. Frees and sets *pbuf to NULL. If anything goes wrong, you return NULL.
- uint8_t bit_read_bit(BitReader *buf): takes in a bitreader pointer, reads a single bit at a time from the bitreader pointer’s values.
- uint8_t bit_read_uint8(BitReader *buf): takes in a bitreader pointer, calls bit_read_bit 8 times. The writing starts with the LSB (least significant or righthand bit).
- uint16_t bit_read_uint16(BitReader *buf): takes in a bitreader pointer, calls bit_read_bit 16 times. The writing starts with the LSB (least significant or righthand bit).
- uint32_t bit_read_uint32(BitReader *buf): takes in a bitreader pointer, calls bit_read_bit 32 times. The writing starts with the LSB (least significant or righthand bit).
- Node *node_create(uint8_t symbol, uint32_t weight): takes in a symbol and weight, and creates a node and sets its symbol and weight. Returns a pointer to the new node, and if there are any errors, returns NULL.
- void node_free(Node **pnode): takes in node double pointer pnode, and frees pnode, and sets pnode to NULL.
- void node_print_tree(Node *tree, char ch, int indentation): takes in the tree, a char ch, and the indentation value. Then prints out the node tree. Used for debugging.
- PriorityQueue *pq_create(void): allocates memory for a priority queue pointer. Returns the pointer to the new pq. Returns NULL if there is an error.
- void pq_free(PriorityQueue **q): takes in priority queue double pointer q, and frees and sets *q to NULL.
- bool pq_is_empty(PriorityQueue *q): takes in the priority queue pointer q, and returns true if true and sets NULL to make sure everything is set otherwise returns false
- bool pq_size_is_1(PriorityQueue *q): takes in a the priority queue pointer q, and if the size of pq is 1 returns true otherwise false
- bool pq_less_than(ListElement *e1, ListElement *e2): takes in ListElement pointer e1 and e2. Compares the weight of e1 and e2 and returns true if the first element is greater than the second one otherwise returns false.
- void enqueue(PriorityQueue *q, Node *tree): takes in a priority queue pointer q and a node pointer tree. Keeps the tree with the lowest weight at the head, and then checks for queue is empty, if the new element is going to be the top element, and whether the new element is going to be placed after an element already in the tree.
- Node *dequeue(PriorityQueue *q): takes in a priority queue pointer q and removes the lowest weight element and returns it. If there are no elements in the queue, then it returns a fatal error.
- void pq_print(PriorityQueue *q): takes in a pointer to priority queue and prints out the queue. This function is given to us.
- uint32_t fill_histogram(FILE *fin, uint32_t *histogram): takes in a file, and pointer histogram. Updates the histogram array, and returns the total file size. For every byte read, you add to the file size.
- Node *create_tree(uint32_t *histogram, uint16_t *num_leaves): takes in a pointer histogram, and num_leaves. Creates and fills the priority queue. Then runs the huffman coding. Dequeues the entry and returns. Also returns the number of leaves in the tree.
- fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length): takes in code pointer code_table, node pointer node, uint64_t code, uint8_t code_length. Then it traverses the tree and fills in the code table. This is a recursive function.
- void huff_compress_file(outbuf, fin, filesize, num_leaves, code_tree, code_table): takes in outbuf, fin, filesize, num_leaves, code_tree, code_table, and then the compresses the value with all of the data that is inputted.
- void dehuff_decompress_file(FILE *fout, BitReader *inbuf): takes in a file and bitreader pointer inbuf. This function reads the tree and decompresses the compressed file, and returns it.


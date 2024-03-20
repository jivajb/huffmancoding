// JIVAJ BRAR bitreader.c
#include "bitreader.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitReader *bit_read_open(const char *filename) {
    BitReader *read = calloc(1, sizeof(BitReader));
    if (read == NULL)
        return NULL;

    FILE *readfile = fopen(filename, "rb");
    if (readfile == NULL) {
        free(read);
        return NULL;
    }

    read->underlying_stream = readfile;
    read->byte = 0;
    read->bit_position = 8;

    return read;
}
void bit_read_close(BitReader **pbuf) {
    if (*pbuf != NULL) {
        if ((fclose((*pbuf)->underlying_stream)) != 0)
            exit(1);
        free(*pbuf);
        *pbuf = NULL;
        if (*pbuf != NULL)
            exit(1);
    }
}
uint32_t bit_read_uint32(BitReader *buf) {
    uint32_t returnval = 0x00000000;
    for (int i = 0; i <= 31; i++) {
        uint8_t temp = bit_read_bit(buf);
        returnval |= (uint32_t) (temp & 1) << i;
    }
    return returnval;
}
uint16_t bit_read_uint16(BitReader *buf) {
    uint16_t returnval = 0x0000;
    for (int i = 0; i <= 15; i++) {
        uint8_t temp = bit_read_bit(buf);
        returnval |= (temp & 1) << i;
    }
    return returnval;
}
uint8_t bit_read_uint8(BitReader *buf) {
    uint8_t returnval = 0x00;
    for (int i = 0; i <= 7; i++) {
        uint8_t temp = bit_read_bit(buf);
        returnval |= (temp & 1) << i;
    }
    return returnval;
}
uint8_t bit_read_bit(BitReader *buf) {
    if (buf->bit_position > 7) {
        int byte = fgetc(buf->underlying_stream);
        if (byte == EOF)
            exit(1);
        buf->byte = (uint8_t) byte;
        buf->bit_position = 0;
    }
    uint8_t returnval = ((buf->byte >> buf->bit_position) & 1);
    buf->bit_position++;
    return returnval;
}

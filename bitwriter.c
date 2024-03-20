// JIVAJ BRAR bitwriter.c
#include "bitwriter.h"

#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitWriter *bit_write_open(const char *filename) {
    FILE *writefile = fopen(filename, "wb");
    BitWriter *write = calloc(1, sizeof(BitWriter));
    write->underlying_stream = writefile;
    write->byte = 0;
    write->bit_position = 0;
    if (writefile == NULL || write == NULL || write->underlying_stream != writefile
        || write->byte != 0 || write->bit_position != 0) {
        return NULL;
    } else {
        return write;
    }
}
void bit_write_close(BitWriter **pbuf) {
    if (*pbuf != NULL) {
        if ((*pbuf)->bit_position > 0) {
            int temp = fputc((*pbuf)->byte, (*pbuf)->underlying_stream);
            if (temp == EOF)
                exit(1);
        }
        if (fclose((*pbuf)->underlying_stream) != 0)
            exit(1);
        free(*pbuf);
        *pbuf = NULL;
    }
}
void bit_write_bit(BitWriter *buf, uint8_t bit) {
    if (7 < (buf->bit_position)) {
        fputc(buf->byte, (buf)->underlying_stream);
        (buf)->byte = 0;
        (buf)->bit_position = 0;
    }
    buf->byte |= (bit & 1) << buf->bit_position;
    buf->bit_position += 1;
}
void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i <= 15; i++) {
        uint8_t returnval = (x >> i) & 1;
        bit_write_bit(buf, returnval);
    }
}
void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i <= 31; i++) {
        uint8_t returnval = (x >> i) & 1;
        bit_write_bit(buf, returnval);
    }
}
void bit_write_uint8(BitWriter *buf, uint8_t byte) {
    for (int i = 0; i <= 7; i++) {
        uint8_t returnval = (byte >> i) & 1;
        bit_write_bit(buf, returnval);
    }
}

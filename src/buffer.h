#ifndef BUFFER_H
#define BUFFER_H
#include "stdint.h"

#define BUFFER_SIZE 256

struct ring_buffer {
    uint8_t buf[BUFFER_SIZE];
    uint8_t begin;
    uint8_t end;
};

void rb_push( struct ring_buffer *rb, char c );
void rb_pop( struct ring_buffer *rb );
char rb_peek( const struct ring_buffer *const rb );
uint8_t rb_is_empty( const struct ring_buffer *const rb );
void rb_init( struct ring_buffer *rb );
#endif

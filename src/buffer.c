#include "buffer.h"

void rb_init( struct ring_buffer *rb )
{
    rb->begin = 0;
    rb->end = 0;
}

void rb_push( struct ring_buffer *rb, char c )
{
    rb->end = ( rb->end + 1 ) % BUFFER_SIZE;
    rb->buf[rb->end] = c;
    if ( rb->end == rb->begin ) rb_pop( rb );
}

void rb_pop( struct ring_buffer *rb )
{
    rb->begin = ( rb->begin + 1 ) % BUFFER_SIZE;
}

char rb_peek( const struct ring_buffer *const rb )
{
    return rb->buf[rb->begin];
}

uint8_t rb_is_empty( const struct ring_buffer *const rb )
{
    if ( rb->end == rb->begin ) return 1;
    return 0;
}
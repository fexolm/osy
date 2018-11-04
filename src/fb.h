#ifndef FB_H
#define FB_H

#include "stdint.h"

void fb_put_b( uint8_t b );
struct io_stream fb_make_io();
void fb_put_s( char const *s );

/* output unsigned integer in decimal format */
void fb_put_ui( uint32_t i );
/* output unsigned integer in hexadecimal format */
void fb_put_ui_hex( uint32_t i );

void fb_clear();
void fb_move_cursor( uint16_t row, uint16_t col );

int fb_init( void );
#endif /* FB_H */

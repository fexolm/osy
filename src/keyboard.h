#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io_stream.h"
#include "stdint.h"

uint32_t kbd_init( void );

struct io_stream kbd_make_io();

#endif /* KEYBOARD_H */

#ifndef IO_STREAM
#define IO_STREAM

#include "stdint.h"

/*
 * @brief io_stream_read contains method of reading from specific IO device
 *
 * @field func - method which would be used to write to IO device
 *
 * @field data - private params which should be passed as first argument to func
 */
struct io_stream_read {
    uint8_t ( *func )( void * );
    void *data;
};

/*
 * @brief io_stream_write contains method of writing to specific io device
 *
 * @field func - method which would be used to read from IO device
 *
 * @field data - private params which should be passed as first argument to func
 */
struct io_stream_write {
    void ( *func )( void *, uint8_t );
    void *data;
};

/*
 * @brief io_stream provides inteface to read/write from IO devices
 */
struct io_stream {
    struct io_stream_read read;
    struct io_stream_write write;
};

#endif

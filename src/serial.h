#ifndef SERIAL_H
#define SERIAL_H

#include "io_stream.h"
#include "stdint.h"

#define SERIAL_COM1 0x3F8
#define SERIAL_COM2 0x2F8
#define SERIAL_COM3 0x3E8
#define SERIAL_COM4 0x2E8

/*!
 * @brief initialize serial port
 *
 * @param com - address of serial port
 */
void serial_init( uint32_t com );

/*!
 * @brief write string to specific serial port
 *
 * @param com - address of serial port
 * @param str - string which would be written to serial port
 */
void serial_push_s( uint32_t com, char *str );

/*!
 * @brief read byte from specific serial port
 *
 * @param com - address of serial port
 */
uint8_t serial_recv_u8( uint32_t com );

/*!
 * @brief returns io_stream which could be used to write/read to serial port
 *
 * @param com - address of serial port
 */
struct io_stream serial_make_io( uint32_t com );

#endif

#ifndef SERIAL_H
#define SERIAL_H
#include "stdint.h"

#define SERIAL_COM1_BASE                0x3F8   
#define SERIAL_COM2_BASE                0x2F8
#define SERIAL_COM3_BASE                0x3E8    
#define SERIAL_COM4_BASE                0x2E8    

/*!
 * @brief initialize serial port
 *
 * @param com - address of serial port
 */
void init_serial(uint32_t com);

/*!
 * @brief write string to specific serial port
 *
 * @param com - address of serial port
 * @param str - string which would be writen to serial port
 */
void serial_push_s(uint32_t com, char *str);

/*!
 * @brief read from address of serial port
 *
 * @param com - address of serial port
 */
uint8_t serial_recv_u8(uint32_t com);

#endif 

#ifndef SERIAL_H
#define SERIAL_H
#include "stdint.h"

#define SERIAL_COM1_BASE                0x3F8   
#define SERIAL_COM2_BASE                0x2F8
#define SERIAL_COM3_BASE                0x3E8    
#define SERIAL_COM4_BASE                0x2E8    

 void init_serial();
 void write_serial(uint32_t com, char *str);

#endif 
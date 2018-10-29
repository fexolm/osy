#include "io.h"
#include "serial.h"

#define SERIAL_DATA_PORT(base)  (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow */
#define SERIAL_LINE_ENABLE_DLAB 0x80

static void set_rate(uint16_t com, uint16_t divisor){
    //baud configuration
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),  divisor & 0x00FF);
}

static void set_line(uint16_t com){
    /*
     * Configure the way of sending data to serial com(x) port
     *
     * Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0х03
     *
     * d - Enables (d = 1) or disables (d = 0) DLAB
     * b - If break control is enabled (b = 1) or disabled (b = 0)
     * prty - The number of parity bits to use
     * s - The number of stop bits to use (s = 0 equals 1, s = 1 equals 1.5 or 2)
     * dl - Describes the length of the data 
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

static void set_buffer(uint16_t com){
    /*
     * Configure FIFO Buffer queue for sending 
      * and receiving data to serial port(14 byte of queue)
     *
     * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
     * Content: | lvl | bs | r | dma | clt | clr | e |
     * Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 | = 0xC7
     *
     * lvl - How many bytes should be stored in the FIFO buffers
     * bs - If the buffers should be 16 or 64 bytes large
     * r - Reserved for future use
     * dma - How the serial port data should be accessed
     * clt - Clear the transmission FIFO buffer
     * clr - Clear the receiver FIFO buffer
     * e - If the FIFO buffer should be enabled or not
     */
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

static void set_modem(uint16_t com){
    /*
     * Configuring the Modem. Hardware flow control via the
       * Ready To Transmit (RTS) and Data Terminal Ready (DTR) pins.
     *
     * Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
     * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
     * Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1   | = 0х03
     *
     * r - Reserved
     * af - Autoflow control enabled
     * lb - Loopback mode (used for debugging serial ports)
     * ao2 - Auxiliary output 2, used for receiving interrupts
     * ao1 - Auxiliary output 1
     * rts - Ready To Transmit
     * dtr - Data Terminal Ready
    */
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

static int fifo_empty(uint32_t com){
    /*
     * The transmit FIFO queue is empty if bit 5 of the line status I/O port is equal to one.
     *
     * Bit:     | 7 6 | 5 | 4 3 2 1 0 |
     * Value:   | 0 0 | 1 | 0 0 0 0 0 | = 0x20
     *
     * return 0 if FIFO queue not empty, 1 - empty
    */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20; 
}

static int recv_able(uint32_t com){
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
}

uint8_t serial_recv_u8(uint32_t com){
    while(recv_able(com) == 0);
    return inb(com);
}

void serial_push_s(uint32_t com, char *str){
    char * i;
    for (i=str; *i; i++) {
        while(fifo_empty(com) == 0);
        outb(com, *i);
    }
}

void init_serial(uint32_t com){
    set_rate(com, 8);
    set_line(com);
    set_buffer(com);
    set_modem(com);
}

    #include "io.h"
    #include "serial.h"

    #define SERIAL_DATA_PORT(base)          (base)
    #define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
    #define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
    #define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
    #define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

    /* Tells the serial port to expect first the highest 8 bits on the data port,
     * then the lowest 8 bits will follow
     */
    #define SERIAL_LINE_ENABLE_DLAB         0x80

    static void serial_configure_baud_rate(uint16_t com, uint16_t divisor){
        //baud configuration
        outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
        outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
        outb(SERIAL_DATA_PORT(com),  divisor & 0x00FF);
    }

    static void serial_configure_line(uint16_t com){
        //0x03 = 0000 0011
        //Configure the way of sending data to serial com(x) port
        outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
    }

    static void serial_buffer_conf(uint16_t com){
        // 0xC7 = 11000111
        //Configure FIFO Buffer queue for sending 
        //and receiving data to serial port(14 byte of queue)
        outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
    }

    static void serial_modem_conf(uint16_t com){
        //Configuring the Modem. Hardware flow control via the 
        //Ready To Transmit (RTS) and Data Terminal Ready (DTR) pins
        outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
    }

    static int serial_is_transmit_fifo_empty(uint32_t com){
        //0x20 = 0010 0000
        //The transmit FIFO queue is empty if bit 5 of the line status I/O port is equal to one.
        return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20; 
        //return 0 if FIFO queue not empty, 1 - empty
    }

    static int serail_is_receiving_able(uint32_t com){
        return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
    }

    //receiving data from I/O port
    int received_serial(uint32_t com){
        while(serail_is_receiving_able(com) == 0);
        return inb(com);
    }

    //sending data to I/O port
    void write_serial(uint32_t com, char *str){
        char * i;
        for (i=str; *i; i++) {
            while(serial_is_transmit_fifo_empty(com) == 0);
            outb(com, *i);
        }
    }


    void init_serial(uint32_t com){
        serial_configure_baud_rate(com, 8);
        serial_configure_line(com);
        serial_buffer_conf(com);
        serial_modem_conf(com);
    }
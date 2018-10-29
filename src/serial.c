    #include "io.h" /* io.h is implement in the section "Moving the cursor" */
    #include "serial.h"

    /* The I/O ports */

    /* All the I/O ports are calculated relative to the data port. This is because
     * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
     * order, but they start at different values.
     */

    #define SERIAL_DATA_PORT(base)          (base)
    #define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
    #define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
    #define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
    #define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

    /* The I/O port commands */

    /* SERIAL_LINE_ENABLE_DLAB:
     * Tells the serial port to expect first the highest 8 bits on the data port,
     * then the lowest 8 bits will follow
     */
    #define SERIAL_LINE_ENABLE_DLAB         0x80

    /** serial_configure_baud_rate:
     *  Sets the speed of the data being sent. The default speed of a serial
     *  port is 115200 bits/s. The argument is a divisor of that number, hence
     *  the resulting speed becomes (115200 / divisor) bits/s.
     *
     *  @param com      The COM port to configure
     *  @param divisor  The divisor
     */
    void serial_configure_baud_rate(uint16_t com, uint16_t divisor);
    void serial_configure_baud_rate(uint16_t com, uint16_t divisor)
    {
        outb(SERIAL_LINE_COMMAND_PORT(com),
             SERIAL_LINE_ENABLE_DLAB);
        outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
        outb(SERIAL_DATA_PORT(com),  divisor & 0x00FF);
    }


    /** serial_configure_line:
     *  Configures the line of the given serial port. The port is set to have a
     *  data length of 8 bits, no parity bits, one stop bit and break control
     *  disabled.
     *
     *  @param com  The serial port to configure
     */
    void serial_configure_line(uint16_t com);
    void serial_configure_line(uint16_t com)
    {
        /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
         * Content: | d | b | prty  | s | dl  |
         * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
         */
        outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
    }

    void serial_buffer_conf(uint16_t com);
    void serial_buffer_conf(uint16_t com){
        outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
    }

    void serial_modem_conf(uint16_t com);
    void serial_modem_conf(uint16_t com){
        outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
    }
    /** serial_is_transmit_fifo_empty:
     *  Checks whether the transmit FIFO queue is empty or not for the given COM
     *  port.
     *
     *  @param  com The COM port
     *  @return 0 if the transmit FIFO queue is not empty
     *          1 if the transmit FIFO queue is empty
     */
    int serial_is_transmit_fifo_empty(uint32_t com)
    {
        /* 0x20 = 0010 0000 */
        return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
    }

    int serail_is_receiving_able(uint32_t com){
        return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
    }

    //receiving data from IO port
    int received_to_serial_port(uint32_t com){
        while(serail_is_receiving_able(com) == 0);
        return inb(com);
    }

    //sending data to IO port
    void write_simbol_to_serial_port(uint32_t com, char str){
        while(serial_is_transmit_fifo_empty(com) == 0);
        outb(com, str);
    }

    void write_serial(uint32_t com, char *str){
        char * i;
        for (i=str; *i; i++) {
            while(serial_is_transmit_fifo_empty(com) == 0);
            outb(SERIAL_COM1_BASE, *i);
        }
        // write_simbol_to_serial_port(SERIAL_COM1_BASE, a);
    }


    void init_serial(){
        uint32_t com  = SERIAL_COM1_BASE;
        serial_configure_baud_rate(com, 8);
        serial_configure_line(com);
        serial_buffer_conf(com);
        serial_modem_conf(com);
    }
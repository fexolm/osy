#include "keyboard.h"
#include "common.h"
#include "fb.h"
#include "interrupt.h"
#include "io.h"
#include "pic.h"

#define KBD_DATA_PORT 0x60
#define KBD_BUFFER_SIZE 512

/* Alphabet */
#define KBD_SC_A 0x1e
#define KBD_SC_B 0x30
#define KBD_SC_C 0x2e
#define KBD_SC_D 0x20
#define KBD_SC_E 0x12
#define KBD_SC_F 0x21
#define KBD_SC_G 0x22
#define KBD_SC_H 0x23
#define KBD_SC_I 0x17
#define KBD_SC_J 0x24
#define KBD_SC_K 0x25
#define KBD_SC_L 0x26
#define KBD_SC_M 0x32
#define KBD_SC_N 0x31
#define KBD_SC_O 0x18
#define KBD_SC_P 0x19
#define KBD_SC_Q 0x10
#define KBD_SC_R 0x13
#define KBD_SC_S 0x1f
#define KBD_SC_T 0x14
#define KBD_SC_U 0x16
#define KBD_SC_V 0x2f
#define KBD_SC_W 0x11
#define KBD_SC_X 0x2d
#define KBD_SC_Y 0x15
#define KBD_SC_Z 0x2c

/* Numeric keys */
#define KBD_SC_1 0x02
#define KBD_SC_2 0x03
#define KBD_SC_3 0x04
#define KBD_SC_4 0x05
#define KBD_SC_5 0x06
#define KBD_SC_6 0x07
#define KBD_SC_7 0x08
#define KBD_SC_8 0x09
#define KBD_SC_9 0x0a
#define KBD_SC_0 0x0b

/* Special keys */
#define KBD_SC_ENTER 0x1c
#define KBD_SC_SPACE 0x39
#define KBD_SC_BS 0x0e
#define KBD_SC_LSHIFT 0x2a
#define KBD_SC_RSHIFT 0x36
#define KBD_SC_DASH 0x0c
#define KBD_SC_EQUALS 0x0d
#define KBD_SC_LBRACKET 0x1a
#define KBD_SC_RBRACKET 0x1b
#define KBD_SC_BSLASH 0x2b
#define KBD_SC_SCOLON 0x27
#define KBD_SC_QUOTE 0x28
#define KBD_SC_COMMA 0x33
#define KBD_SC_DOT 0x34
#define KBD_SC_FSLASH 0x35
#define KBD_SC_TILDE 0x29
#define KBD_SC_CAPSLOCK 0x3a
#define KBD_SC_TAB 0x0f

static char kbd_scan_code_to_ascii( uint8_t sc );
static uint8_t kbd_read_scan_code( void );

static void keyboard_handle_interrupt( cpu_state_t state, idt_info_t info,
                                       stack_state_t exec )
{
    UNUSED_ARGUMENT( state );
    UNUSED_ARGUMENT( info );
    UNUSED_ARGUMENT( exec );

    /* DO NOT MODIFY kbd_buffer.head in this function,
     * it will introcude race conditions!
     */

    uint8_t code = kbd_read_scan_code();

    char c = kbd_scan_code_to_ascii( code );

    if ( c != -1 ) {
        fb_put_b( c );
    }
    pic_acknowledge();
}

uint32_t kbd_init( void )
{
    register_interrupt_handler( KBD_INT_IDX, keyboard_handle_interrupt );
    return 0;
}

uint8_t kbd_read_scan_code( void )
{
    return inb( KBD_DATA_PORT );
}

static char kbd_scan_code_to_ascii( uint8_t scan_code )
{
    char ch = -1;

    switch ( scan_code ) {
    case KBD_SC_A:
        ch = 'a';
        break;
    case KBD_SC_B:
        ch = 'b';
        break;
    case KBD_SC_C:
        ch = 'c';
        break;
    case KBD_SC_D:
        ch = 'd';
        break;
    case KBD_SC_E:
        ch = 'e';
        break;
    case KBD_SC_F:
        ch = 'f';
        break;
    case KBD_SC_G:
        ch = 'g';
        break;
    case KBD_SC_H:
        ch = 'h';
        break;
    case KBD_SC_I:
        ch = 'i';
        break;
    case KBD_SC_J:
        ch = 'j';
        break;
    case KBD_SC_K:
        ch = 'k';
        break;
    case KBD_SC_L:
        ch = 'l';
        break;
    case KBD_SC_M:
        ch = 'm';
        break;
    case KBD_SC_N:
        ch = 'n';
        break;
    case KBD_SC_O:
        ch = 'o';
        break;
    case KBD_SC_P:
        ch = 'p';
        break;
    case KBD_SC_Q:
        ch = 'q';
        break;
    case KBD_SC_R:
        ch = 'r';
        break;
    case KBD_SC_S:
        ch = 's';
        break;
    case KBD_SC_T:
        ch = 't';
        break;
    case KBD_SC_U:
        ch = 'u';
        break;
    case KBD_SC_V:
        ch = 'v';
        break;
    case KBD_SC_W:
        ch = 'w';
        break;
    case KBD_SC_X:
        ch = 'x';
        break;
    case KBD_SC_Y:
        ch = 'y';
        break;
    case KBD_SC_Z:
        ch = 'z';
        break;
    case KBD_SC_0:
        ch = '0';
        break;
    case KBD_SC_1:
        ch = '1';
        break;
    case KBD_SC_2:
        ch = '2';
        break;
    case KBD_SC_3:
        ch = '3';
        break;
    case KBD_SC_4:
        ch = '4';
        break;
    case KBD_SC_5:
        ch = '5';
        break;
    case KBD_SC_6:
        ch = '6';
        break;
    case KBD_SC_7:
        ch = '7';
        break;
    case KBD_SC_8:
        ch = '8';
        break;
    case KBD_SC_9:
        ch = '9';
        break;
    case KBD_SC_ENTER:
        ch = '\n';
        break;
    case KBD_SC_SPACE:
        ch = ' ';
        break;
    case KBD_SC_BS:
        ch = 8;
        break;
    case KBD_SC_DASH:
        ch = '-';
        break;
    case KBD_SC_EQUALS:
        ch = '=';
        break;
    case KBD_SC_LBRACKET:
        ch = '[';
        break;
    case KBD_SC_RBRACKET:
        ch = ']';
        break;
    case KBD_SC_BSLASH:
        ch = '\\';
        break;
    case KBD_SC_SCOLON:
        ch = ';';
        break;
    case KBD_SC_QUOTE:
        ch = '\'';
        break;
    case KBD_SC_COMMA:
        ch = ',';
        break;
    case KBD_SC_DOT:
        ch = '.';
        break;
    case KBD_SC_FSLASH:
        ch = '/';
        break;
    case KBD_SC_TILDE:
        ch = '`';
        break;
    case KBD_SC_TAB:
        ch = '\t';
        break;
    default:
        return -1;
    }

    return ch;
}

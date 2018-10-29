#include "../lib/multiboot/multiboot.h"
#include "common.h"
#include "constants.h"
#include "fb.h"
#include "gdt.h"
#include "idt.h"
#include "interrupt.h"
#include "io.h"
#include "keyboard.h"
#include "pic.h"
#include "serial.h"
#include "tss.h"

static uint32_t kinit()
{
    disable_interrupts();

    uint32_t tss_vaddr = tss_init();
    gdt_init( tss_vaddr );
    idt_init();
    pic_init();
    kbd_init();
    fb_init();
    serial_init( SERIAL_COM1 );
    enable_interrupts();
    return 0;
}

static void start_init()
{
    fb_clear();
}

typedef void ( *call_module_t )( void );

int kmain( const multiboot_info_t *mbinfo, uint32_t kernel_virtual_start,
           uint32_t kernel_virtual_end, uint32_t kernel_physical_start,
           uint32_t kernel_physical_end, uint32_t *kernel_pdt,
           uint32_t *kernel_pt )
{
    UNUSED_ARGUMENT( mbinfo );
    UNUSED_ARGUMENT( kernel_virtual_start );
    UNUSED_ARGUMENT( kernel_virtual_end );
    UNUSED_ARGUMENT( kernel_physical_start );
    UNUSED_ARGUMENT( kernel_physical_end );
    UNUSED_ARGUMENT( kernel_pdt );
    UNUSED_ARGUMENT( kernel_pt );

    kinit();
    start_init();

    serial_push_s( SERIAL_COM1, "Test serial" );

#if 0
	multiboot_module_t * mod = (multiboot_module_t *)mbinfo->mods_addr;
	call_module_t start_program = (call_module_t)mod->mod_start;
	start_program();
#endif
    return 0xDEADBEEF;
}

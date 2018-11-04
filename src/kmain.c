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
#include "pfa.h"
#include "memory.h"
#include "paging.h"
#define TO_VIRTUAL(x) (((uint32_t)(x) + KERNEL_START_VADDR))

static uint32_t kinit(const multiboot_info_t *mbinfo,
                      const struct kernel_memory *mem,
                      uint32_t kernel_pdt_vaddr,
                      uint32_t kernel_pt_vaddr)

{
    disable_interrupts();

    uint32_t tss_vaddr = tss_init();
    gdt_init( tss_vaddr );
    idt_init();

    pic_init();
    kbd_init();
    fb_init();
    serial_init( SERIAL_COM1 );

    if(paging_init(kernel_pdt_vaddr, kernel_pt_vaddr) != 0) {
        fb_put_s("ERROR PADDING");
    }

    if(pfa_init(mbinfo, mem) != 0) {
        fb_put_s("ERROR PFA");
    }

    enable_interrupts();
    return 0;
}

typedef void ( *call_module_t )( void );

int kmain( multiboot_info_t *mbinfo,
           struct kernel_memory mem,
           uint32_t kernel_pdt_vaadr,
           uint32_t kernel_pt_vaadr )
{
    mbinfo = (multiboot_info_t *)TO_VIRTUAL(mbinfo);
    mbinfo->mmap_addr = TO_VIRTUAL(mbinfo->mmap_addr);

    kinit(mbinfo, &mem, kernel_pdt_vaadr, kernel_pt_vaadr);
#if 0
	multiboot_module_t * mod = (multiboot_module_t *)mbinfo->mods_addr;
	call_module_t start_program = (call_module_t)mod->mod_start;
	start_program();
#endif
    return 0xDEADBEEF;
}

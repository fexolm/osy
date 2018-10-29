#include "gdt.h"
#include "pic.h"
#include "idt.h"
#include "interrupt.h"
#include "constants.h"
#include "tss.h"
#include "keyboard.h"
#include "fb.h"
#include "io.h"
#include "../lib/multiboot/multiboot.h"
#include "common.h"
#include "serial.h"

static uint32_t kinit()
{
	disable_interrupts();

	uint32_t tss_vaddr = tss_init();
	gdt_init(tss_vaddr);
	idt_init();
	pic_init();
	kbd_init();
	fb_init();

	enable_interrupts();
	return 0;
}

static void start_init()
{
	fb_clear();
}

typedef void(*call_module_t)(void);
int kmain(const multiboot_info_t *mbinfo)
{

	kinit();
	start_init();

	init_serial();

	multiboot_module_t * mod = (multiboot_module_t *)mbinfo->mods_addr;
	call_module_t start_program = (call_module_t)mod->mod_start;

	start_program();
	fb_put_ui(mbinfo->mods_count);
	fb_put_ui(mod->cmdline);

	while (1);
	return 0xDEADBEEF;
}

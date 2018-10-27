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

int kmain(
	const multiboot_info_t *mbinfo,
	uint32_t kernel_virtual_start,
	uint32_t kernel_virtual_end,
	uint32_t kernel_physical_start,
	uint32_t kernel_physical_end,
	uint32_t *kernel_pdt,
	uint32_t *kernel_pt
)
{
	UNUSED_ARGUMENT(mbinfo);

	kinit();
	start_init();
	fb_put_ui_hex(kernel_virtual_start);
	fb_put_s(" ");
	fb_put_ui_hex(kernel_virtual_end);
	fb_put_s("\n");

	fb_put_ui_hex(kernel_physical_start);
	fb_put_s(" ");
	fb_put_ui_hex(kernel_physical_end);
	fb_put_s("\n");
	for (int i = 0; i < 1024; i++) {
		if (kernel_pdt[i] != 0) {
			fb_put_ui_hex(kernel_pdt[i]);
			fb_put_s(" ");
			fb_put_ui(i);
		}
	}
	fb_put_s("\n");

	for (int i = 0; i < 10; i++) {
		fb_put_ui_hex(kernel_pt[i]);
		fb_put_s(" ");
	}

	//multiboot_module_t * mod = (multiboot_module_t *)mbinfo->mods_addr;
	//call_module_t start_program = (call_module_t)mod->mod_start;
	//start_program();

	//while (1);
	return 0xDEADBEEF;
}

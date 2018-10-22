#include "gdt.h"
#include "pic.h"
#include "idt.h"
#include "interrupt.h"
#include "constants.h"
#include "tss.h"
#include "keyboard.h"
#include "fb.h"
#include "io.h"

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

	fb_put_b(3);
	while (1);
}

int kmain()
{
	kinit();

	start_init();

	return 0xDEADBEEF;
}

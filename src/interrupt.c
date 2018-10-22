#include "interrupt.h"
#include "pic.h"
#include "constants.h"
#include "common.h"
#include "fb.h"
#include "io.h"
static interrupt_handler_t interrupt_handlers[IDT_NUM_ENTRIES];

uint32_t register_interrupt_handler(uint32_t interrupt,
                                    interrupt_handler_t handler)
{
    if (interrupt > 255) {
        return 1;
    }
    if (interrupt_handlers[interrupt] != 0) {
        return 1;
    }

    interrupt_handlers[interrupt] = handler;
    return 0;
}
int i = 0;
void interrupt_handler(cpu_state_t state, idt_info_t info, stack_state_t exec)
{
    if (interrupt_handlers[info.idt_index] != 0) {
        interrupt_handlers[info.idt_index](state, info, exec);
    } else {
    }
	pic_acknowledge();
}

#ifndef PFA_H
#define PFA_H

#include "../lib/multiboot/multiboot.h"
#include "memory.h"

uint32_t pfa_init(const multiboot_info_t *mbinfo,
                  const struct kernel_memory *mem );

uint32_t pfa_allocate(uint32_t num_page_frames);
void pfa_free(uint32_t paddr);
void pfa_free_cont(uint32_t paddr, uint32_t n);


#endif /* PFA_H */

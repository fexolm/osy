#ifndef PFA_H
#define PFA_H

#include "../lib/multiboot/multiboot.h"
#include "memory.h"

void pfa_init( const multiboot_info_t *mbinfo,
               const struct kernel_memory *mem );

#endif /* PFA_H */

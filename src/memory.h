#ifndef MEMORY_H
#define MEMORY_H
#include "stdint.h"

struct kernel_memory {
    uint32_t kernel_virtual_start;
    uint32_t kernel_virtual_end;
    uint32_t kernel_physical_start;
    uint32_t kernel_physical_end;
} __attribute__( ( packed ) );


uint32_t align_up(uint32_t n, uint32_t a);

uint32_t align_down(uint32_t n, uint32_t a);


#endif /* MEMORY_H */

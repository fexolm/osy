#include "pfa.h"
#include "common.h"
#include "fb.h"
#include "memory.h"

#define MAX_NUM_MEMORY_MAP 100

struct memory_map {
    uint32_t addr;
    uint32_t len;
};

static struct memory_map mmap[MAX_NUM_MEMORY_MAP];

static uint32_t mmap_len;

void pfa_init( const multiboot_info_t *mbinfo, const struct kernel_memory *mem )
{
    if ( ( mbinfo->flags & 0x00000020 ) == 0 ) {
        fb_put_s( "No memory map\n" );
        return;
    }

    multiboot_memory_map_t *entry = (multiboot_memory_map_t *)mbinfo->mmap_addr;
    int i = 0;
    while ( (uint32_t)entry < mbinfo->mmap_addr + mbinfo->mmap_length ) {
        if ( entry->type != MULTIBOOT_MEMORY_AVAILABLE ) {
            continue;
        }
        uint32_t addr = (uint32_t)entry->addr;
        uint32_t len = entry->len;

        if ( addr <= mem->kernel_physical_start ) {
            if ( addr + len < mem->kernel_physical_end ) {
                continue;
            }
            addr = mem->kernel_physical_end;
            len = len - mem->kernel_virtual_end;
        }

        if ( addr > ONE_MB ) {
            mmap[i].addr = align_up(addr, FOUR_KB);
            mmap[i].len = align_down(len - (mmap[i].addr - addr), FOUR_KB);
        }

        entry = (multiboot_memory_map_t *)( (unsigned int)entry + entry->size +
                                            sizeof( entry->size ) );
    }
    mmap_len = i;
}

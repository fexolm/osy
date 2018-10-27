%include "src/constants.inc"

global loader    
global kernel_stack

extern kmain
extern kernel_virtual_end              
extern kernel_virtual_start
extern kernel_physical_end
extern kernel_physical_start

MAGIC_NUMBER		equ 0x1BADB002        
ALIGN_MODULES		equ 1<<0		
MEMINFO				equ 1<<1
FLAGS				equ ALIGN_MODULES

CHECKSUM			equ -(MAGIC_NUMBER + FLAGS) 

KERNEL_PT_CFG       equ 00000000000000000000000000001011b
KERNEL_PDT_ID_MAP   equ 00000000000000000000000010001011b

section .data
align 4096
kernel_pt:
    times 1024 dd 0

kernel_pdt:
    dd KERNEL_PDT_ID_MAP
    times 1023 dd 0

section .data
align 4
grub_magic_number:
    dd 0
grub_multiboot_info:
    dd 0

section .bss
align 4                                    
kernel_stack:                             
    resb KERNEL_STACK_SIZE                

section .text                       ; start of the text (code) section
align 4                             ; the code must be 4 byte aligned
    dd MAGIC_NUMBER                 ; write the magic number
    dd ALIGN_MODULES                ; write the align modules instruction
    dd CHECKSUM                     ; write the checksum
        

loader:                       
    mov ecx, (grub_magic_number - KERNEL_START_VADDR)
    mov [ecx], eax
    mov ecx, (grub_multiboot_info - KERNEL_START_VADDR)
    mov [ecx], ebx

set_up_kernel_pdt:
    ; set up kernel_pdt to point to kernel_pt
    mov ecx, (kernel_pdt - KERNEL_START_VADDR + KERNEL_PDT_IDX*4)
    mov edx, (kernel_pt - KERNEL_START_VADDR)
    or  edx, KERNEL_PT_CFG
    mov [ecx], edx

set_up_kernel_pt:
    mov eax, (kernel_pt - KERNEL_START_VADDR)
    mov ecx, KERNEL_PT_CFG

; kernel memory init
.loop:
    mov [eax], ecx
    add eax, 4
    add ecx, FOUR_KB
    cmp ecx, kernel_physical_end
    jle .loop

enable_paging:
    mov ecx, (kernel_pdt - KERNEL_START_VADDR)
    and ecx, 0xFFFFF000     ; we only care about the upper 20 bits
    or  ecx, 0x08           ; PWT, enable page write through?
    mov cr3, ecx            ; load pdt

    mov ecx, cr4            ; read current config from cr4
    or  ecx, 0x00000010     ; set bit enabling 4MB pages
    mov cr4, ecx            ; enable it by writing to cr4

    mov	ecx, cr0	        ; read current config from cr0
    or  ecx, 0x80000000	    ; the highest bit controls paging
    mov cr0, ecx	        ; enable paging by writing config to cr0

    lea ecx, [higher_half]  ; store the address higher_half in ecx
    jmp ecx                 ; now we jump into 0xC0100000

; code executing from here on uses the page table, and is accessed through
; the upper half, 0xC0100000

higher_half:
    mov [kernel_pdt], DWORD 0
    invlpg [0]
    mov esp, kernel_stack+KERNEL_STACK_SIZE  ; set up the stack

push kernel_pt
push kernel_pdt
push kernel_physical_end
push kernel_physical_start
push kernel_virtual_end
push kernel_virtual_start
push ebx
call kmain


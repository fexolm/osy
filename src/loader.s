global loader    
global kernel_stack

extern kmain

MAGIC_NUMBER	equ 0x1BADB002        
ALIGN_MODULES   equ 1<<0		
MEMINFO			equ 1<<1
FLAGS			equ ALIGN_MODULES 

CHECKSUM    equ -(MAGIC_NUMBER + FLAGS) 

KERNEL_STACK_SIZE equ 4096               

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
    mov eax, 0xCAFEBABE         
	mov esp, kernel_stack + KERNEL_STACK_SIZE

push ebx
call kmain


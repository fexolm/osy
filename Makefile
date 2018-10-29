CC = gcc
AS = nasm

BINARY_DIR = bin/
ISO_DIR = $(BINARY_DIR)iso/
SRC_DIR = src/

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
ASFLAGS = -f elf
LDFLAGS = -T $(SRC_DIR)link.ld -melf_i386

OBJ = gdt.o gdt_asm.o idt.o idt_asm.o interrupt.o interrupt_asm.o io.o kmain.o loader.o pic.o tss.o tss_asm.o keyboard.o fb.o serial.o

all: prepare kernel.elf os.iso

prepare: clean
	mkdir $(BINARY_DIR)
	cp -r iso $(BINARY_DIR)

kernel.elf: $(OBJ)
	ld $(LDFLAGS) $(addprefix $(BINARY_DIR),$(OBJ)) -o $(BINARY_DIR)kernel.elf

os.iso:
	cp $(BINARY_DIR)kernel.elf $(ISO_DIR)boot/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o$(BINARY_DIR)os.iso $(ISO_DIR)

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS)  $< -o $(BINARY_DIR)$@

%.o: $(SRC_DIR)%.s
	$(AS) $(ASFLAGS) $< -o $(BINARY_DIR)$@

clean:
	rm -rf $(BINARY_DIR)

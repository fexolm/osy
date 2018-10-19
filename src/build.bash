mkdir ../bin
cp -r ../iso ../bin
nasm -f elf32 loader.s -o ../bin/loader.o
gcc -std=c11 -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c kmain.c -o ../bin/kmain.o
ld -T link.ld -melf_i386 ../bin/*.o -o ../bin/iso/boot/kernel.elf

cd ../bin

genisoimage -R                                  \
                -b boot/grub/stage2_eltorito    \
                -no-emul-boot                   \
                -boot-load-size 4               \
                -A os                           \
                -input-charset utf8             \
                -quiet                          \
                -boot-info-table                \
                -o os.iso                       \
                iso


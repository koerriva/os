build:
	mkdir -p out/iso/boot/grub
	nasm -f elf64 src/asm/multiboot_header.asm -o out/multiboot_header.o
	nasm -f elf64 src/asm/boot.asm -o out/boot.o
	nasm -f elf64 src/asm/lib.asm -o out/lib.o
	nasm -f elf64 src/asm/vga.asm -o out/vga.o
	gcc -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
	 -c src/c/kernel.c -o out/kernel.o 
	ld -n -o out/kernel.bin -T linker.ld out/*.o
	cp src/resources/grub.cfg out/iso/boot/grub
	cp out/kernel.bin out/iso/boot
	grub-mkrescue -o out/os.iso out/iso
	qemu-system-x86_64 -cdrom out/os.iso

clean:
	rm -rf out
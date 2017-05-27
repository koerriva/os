kernel.bin: boot.asm multiboot_header.asm
	nasm -f elf64 multiboot_header.asm
	nasm -f elf64 boot.asm
	nasm -f elf64 long_mode_init.asm
	ld -n --gc-sections -o kernel.bin -T linker.ld multiboot_header.o boot.o long_mode_init.o libos.a
	rm -rf *.o
os.iso: kernel.bin grub.cfg
	mkdir -p iso/boot/grub
	cp grub.cfg iso/boot/grub
	cp kernel.bin iso/boot
	grub-mkrescue -o os.iso iso
	rm -rf iso
run: os.iso
	qemu-system-x86_64 -enable-kvm -cdrom os.iso -m 8G
clean:
	rm os.iso kernel.bin

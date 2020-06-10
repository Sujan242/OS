
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
# as the compiler assumes that the OS will take care of dynamic memory allocation,exception handling
# we hace to explicitly tell state that no such services,libraries are provided yet
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o gdt.o port.o interruptstubs.o interrupts.o keyboard.o kernel.o


run: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm 'My Operating System' &

%.o: %.cpp		# instructions to convert .cpp files to .o file
	gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s		# instructions to convert .s files to .o file
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)	# to generate .bin file from linker.ld file
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin		#creating .iso file
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

install: mykernel.bin	#installing .bin file, copying to root
	sudo cp $< /boot/mykernel.bin

.PHONY: clean		# removing all objects,.bin,.iso file
clean:
	rm -f $(objects) mykernel.bin mykernel.iso

.set MAGIC, 0x1badb002  #used to tell the boot loader that kernel is being loaded (called magicnumber which has to be there in kernel.bin file)
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot     #To put the above variables in loader file
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


.section .text
.extern kernelMain
.extern callConstructors
.global loader


loader:                 #loads the function kernelMain
    mov $kernel_stack, %esp
    call callConstructors
    push %eax
    push %ebx
    call kernelMain


_stop:      #to ensure infinte loop once booted
    cli
    hlt
    jmp _stop


.section .bss
.space 2*1024*1024; #minimum space before which kernel code can be loaded onto the RAM
kernel_stack:       #stack is built leftward and there maybe a space constraint


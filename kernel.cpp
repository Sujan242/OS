
#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"


void printf(char* str)  //we have to define our own printf function as there are no libraries yet
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;  //OS dynamically links the function call to the library


    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }
         // if number of print statements exceed the screen size, we clear all and start printing again
        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()  //initialise objects
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{   //g++ has a different naming convention
    //hence while writing into .o file changes name
    printf("Hello World!");

    GlobalDescriptorTable gdt;  //to prevent that extern is used
    InterruptManager interrupts(0x20, &gdt);
    KeyboardDriver keyboard(&interrupts);
    interrupts.Activate();

    while(1);
}

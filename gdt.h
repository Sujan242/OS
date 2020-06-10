// used to describe the memory allocated to processes in the RAM using memory segments
// used to instruct the CPU to shift from one process code block to another
// unless specified the above doesn't happen automatically 
// assuming the the GlobalDescriptorTable has 8 Byte records

#ifndef __GDT_H
#define __GDT_H

    #include "types.h"
    
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {           // describes how the memory is allocated in the RAM for a process
                private:        // ie. how much bits for the code,data,access rights
                    uint16_t limit_lo;      // 2 Byte for pointer
                    uint16_t base_lo;       // 2 Byte for pointer
                    uint8_t base_hi;        // 1 Byte extension for the pointer
                    uint8_t type;           // 1 Byte for access rights
                    uint8_t limit_hi;
                    uint8_t base_vhi;

                public:
                    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                    uint32_t Base();
                    uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            uint16_t CodeSegmentSelector();     //used to initialise the code segment of the process
            uint16_t DataSegmentSelector();     // used to initialise the data segment of the process
    };

#endif

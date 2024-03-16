ENTRY(_start)

MEMORY
{
        main_flash (rx) : ORIGIN = 0x08000000, LENGTH = 0x2000
        sram (rw!x): ORIGIN = 0x0c000000, LENGTH = 0x2000
}

SECTIONS
{
        .vectors : {
                __vector_base = .;
                *(.exceptions) *(.interrupts)
		. = ALIGN(0x100);
        } > main_flash =0xEE

        .hdrs : {
                KEEP(*(.romhdr))
                . = ALIGN(0x10);
        } > main_flash =0xEE

        .text : {
                *(.text) 
                *(.text.*)
                . = ALIGN(0x10);
        } > main_flash =0xEE

        .rodata : {
                *(.rodata) 
                *(.rodata.*)
        
                /* put the libc contructor/destructor sections in rodata */
                *(.init_array)
                *(.fini_array)
                . = ALIGN(0x10);
        } > main_flash =0xEE

        .init.fw : {
                __kconsole_init_start = .;
                *(.init.kconsole)
                __kconsole_init_end = .;

                __device_initcall_start = .;
                *(.init.call)
                __device_initcall_end = .;
                . = ALIGN(0x10);
        } > main_flash =0xEE
        
        .data : AT ( ADDR(.init.fw) + SIZEOF(.init.fw) ) {
                __rom_data = LOADADDR(.data); __ram_data = .;

                *(.data) 
                *(.data.*)
        } > sram
        __data_size = SIZEOF(.data);

        . = ALIGN(8);
        .bss : {
                __bss_start__ = .;
                *(.bss)
                *(.bss.*)
                . = ALIGN(8);
                __bss_end__ = .;
        } > sram

        .stack : {
                /* nacuna??? 3k stack space */
                . = . + 0xc00;
        } > sram
        . = ALIGN(8);
        __initial_sp = .;
        __stack = .;
        
}


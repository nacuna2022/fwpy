#define __ASSEMBLY__

        .thumb
        .syntax unified

        .extern __initial_sp
        .globl __reset
        .extern _start
        .align  3
        .section .exceptions, "ax"

        .long   arm_reset
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0
        //.long   systick_handler

        /* max possible interrupt vectors ARMv7 can have is 496. but for now,
         * lets cap the max to 128 external interrupts.
         * nacuna??? find a way to let boards define their own interrupt 
         * vectors - probably in a separate source file instead of here.
         */
         .align 3
         .extern arm_irq
         .section .interrupts, "ax"

        .long   0      // 0

        .align 3
        .section .text, "ax"
 
        .globl _start
 _start:
arm_reset:
        b       .


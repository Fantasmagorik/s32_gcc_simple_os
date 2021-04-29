
.syntax unified
.cpu cortex-m3
.thumb
.include "stm32f103x.s"

.global PendSV_Handler
.global TASK_COUNT

.equ TASK_COUNT, 10
.equ TASK_STACK_SIZE, 20

.section .bss

ACTIVE_TASK_ID: .word 0

    TOP_OF_STACK = .;
.space TASK_STACK_SIZE * TASK_COUNT


.section .code.task_scheduller
.type PendSV_Handler, %function

PendSV_Handler:
    ldr r0, =TOP_OF_STACK
    ldr r1, =ACTIVE_TASK_ID
    ldr r1, [r1]
    ldr r2, =TASK_STACK_SIZE
    mla r3, r2, r1, r0  //r3 is address of current task`s STACK
    //ldr r0, =TOP_OF_STACK + =ACTIVE_TASK_ID * =TASK_STACK_SIZE


.intel_syntax noprefix

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16

stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
  // Stack setup
  lea esp, [stack_top]

  // Jump to main kernel
  call kernel_main

  // Enter infinite loop in the rare case the kernel returns
  cli
wtf_something_went_wrong:
  hlt
  jmp wtf_something_went_wrong

.size _start, . - _start

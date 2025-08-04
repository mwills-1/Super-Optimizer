.text
.globl _add_numbers
.align 2
_add_numbers:
    add w0, w0, w1    // Add w1 to w0, result in w0 (32-bit integers)
    ret
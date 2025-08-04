.text
.globl _multiply
.align 2
_multiply:
    mul w0, w0, w1    // Multiply w0 by w1, result in w0
    ret 
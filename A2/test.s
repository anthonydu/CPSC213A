.pos 0x100 # load a and b
        ld $a, r0
        ld 0(r0), r1
        ld 4(r0), r2
        halt

.pos 0x200 # load a and b
        ld $a, r0
        ld $0, r1
        ld $1, r2
        ld (r0, r1, 4), r1
        ld (r0, r2, 4), r2
        halt

.pos 0x300 # swap a and b
        ld $a, r0
        ld (r0), r1
        ld $b, r2
        ld (r2), r2
        st r2, (r0)
        st r1, 4(r0)
        halt

.pos 0x400 # swap a and b
        ld $a, r0
        ld (r0), r1
        ld $b, r2
        ld (r2), r2
        ld $0, r3
        ld $1, r4
        st r2, (r0, r3, 4)
        st r1, (r0, r4, 4)
        halt

.pos 0x600 # all ALU's
        ld $0x0f0f0f0f, r0
        mov r0, r1
        inc r0
        inca r1
        add r0, r1
        and r1, r0
        dec r0
        deca r1
        not r1
        not r1
        halt

.pos 0x700 # shl, shr
        ld $0x0f0f0f0f, r0
        shl $3, r0
        shr $5, r0
        halt



.pos 0x1000
a:      .long 0x0f0f0f0f
b:      .long 0xf0f0f0f0

.pos 0x100
        ld      $m, r0          # r0 = &m
        ld      (r0), r1        # r1 = m
        mov     r1, r2          # r2 = m
        ld      $5, r7          # r7 = 5
        add     r7, r2          # r2 += 5
        shl     $4, r2          # r2 << 4
        and     r1, r2          # r2 &= r1
        ld      $0x0, r7        # r6 = 0
div8:   deca    r2              # r2 -= 4
        deca    r2              # r2 -= 4
        inc     r7              # r7 += 1
        beq     r2, skpd        # if r2 == 0, skip dec
        bgt     r2, div8        # if r2 > 0, keep looping
        dec     r7              # r7 -= 1
skpd:   ld      $i, r0          # r0 = &i
        st      r7, (r0)        # r7 => i
        halt
.pos 0x1000
i:      .long   0xffffffff
m:      .long   0xf0f0f0f0
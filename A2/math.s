.pos 0x100
        ld      $m, r0          # r0 = &m
        ld      (r0), r1        # r1 = m
        mov     r1, r2          # r2 = m
        inc     r2              # r2 += 1
        inca    r2              # r2 += 4
        shl     $4, r2          # r2 << 4
        and     r1, r2          # r2 &= r1
        shr     $3, r2          # r2 /= 8
        ld      $i, r0          # r0 = *i
        st      r2, (r0)        # r7 => i
        halt
.pos 0x1000
i:      .long   0xffffffff
m:      .long   0xf0f0f0f0
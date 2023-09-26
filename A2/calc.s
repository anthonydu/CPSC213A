.pos 0x100
calc:   ld      $c, r0
        ld      (r0), r0
        ld      $a, r1
        ld      (r1, r0, 4), r2
        inca    r0
        dec     r0
        ld      (r1, r0, 4), r3
        add     r3, r2
        ld      $i, r0
        st      r2, (r0)
        ld      $0xf0, r3
        and     r3, r2 
        ld      $x, r0
        st      r2, (r0)
        halt
.pos 0x1000
c:      .long   5
x:      .long   0xffffffff
i:      .long   0xffffffff
.pos 0x1100
a:      .long   0xffffffff      # 0
        .long   0xffffffff      # 1
        .long   0xffffffff      # 2
        .long   0xffffffff      # 3
        .long   0xffffffff      # 4
        .long   99              # 5
        .long   0xffffffff      # 6
        .long   0xffffffff      # 7
        .long   8               # 8
        .long   0xffffffff      # 9

.pos 0x100
        # i = u[7];
        ld      $u, r0          # r0 = address of u
        ld      28(r0), r1      # r1 = u[7]
        ld      $i, r2          # r2 = address of i
        st      r1, (r2)        # i = u[7]

        # i = u[i]
        ld      $i, r0          # r0 = address of i
        ld      (r0), r1        # r1 = i
        ld      $u, r2          # r2 = address of u
        ld      (r2, r1, 4), r3 # r3 = u[i]
        st      r3, (r0)        # i = u[i]

        # f = &a
        ld      $a, r0          # r0 = address of a
        ld      $f, r1          # r1 = address of f
        st      r0, (r1)        # f = &a

        # *f = 4
        ld      $4, r0          # r0 = 4
        ld      $f, r1          # r1 = address of f
        ld      (r1), r2        # r2 = address of *f
        st      r0, (r2)        # *f = 4

        # f = &u[u[1]]
        ld      $u, r0          # r0 = address of u
        ld      4(r0), r1       # r1 = u[1]
        shl     $2, r1          # r1 *= 4
        add     r0, r1          # r1 = &u[u[1]]
        ld      $f, r2          # r2 = address of f
        st      r1, (r2)        # f = &u[u[1]]

        # *f = *f + u[9]
        ld      $u, r0          # r0 = address of u
        ld      36(r0), r1      # r1 = u[9]
        ld      $f, r2          # r2 = address of f
        ld      (r2), r3        # r3 = address of *f
        ld      (r3), r4        # r4 = *f
        add     r4, r1          # r1 = *f + u[9]
        st      r1, (r3)        # *f = *f + u[9]
        
        halt

.pos 0x1000
i:      .long   0               # i
a:      .long   0               # a
f:      .long   0               # *f
u:      .long   0               # u[0]
        .long   1               # u[1]
        .long   2               # u[2]
        .long   3               # u[3]
        .long   4               # u[4]
        .long   5               # u[5]
        .long   6               # u[6]
        .long   7               # u[7]
        .long   8               # u[8]
        .long   9               # u[9]
        
.pos 0x100
        # a = 3
        ld      $3, r0          # r0 = 3
        ld      $a, r1          # r1 = address of a
        st      r0, (r1)        # a = 3

        # p = &a
        ld      $a, r0          # r0 = address of a
        ld      $p, r1          # r1 = address of p
        st      r0, (r1)        # p = &a

        # *p = *p - 1
        ld      $p, r0          # r0 = address of p
        ld      (r0), r1        # r1 = p
        ld      (r1), r2        # r2 = *p
        dec     r2              # r2 -= 1
        st      r2, (r1)        # *p = *p - 1

        # p = &b[0]
        ld      $b, r0          # r0 = address of b
        ld      $p, r1          # r0 = address of p
        st      r0, (r1)        # p = &b[0]

        # p++
        ld      $p, r0          # r0 = address of p
        ld      (r0), r1        # r1 = p
        inca    r1              # r1 += 4
        st      r1, (r0)        # p++

        # p[a] = b[a]
        ld      $a, r0          # r0 = address of a
        ld      (r0), r1        # r1 = a
        ld      $b, r2          # r2 = address of b
        ld      (r2, r1, 4), r3 # r3 = b[a]
        ld      $p, r4          # r4 = address of p
        ld      (r4), r5        # r5 = p
        shl     $2, r1          # r1 *= 4
        add     r1, r5          # r4 = address of p[a]
        st      r3, (r5)        # p[a] = b[a]

        # *(p+3) = b[0]
        ld      $b, r0          # r0 = address of b
        ld      (r0), r1        # r1 = b[0]
        ld      $12, r2         # r2 = 12
        ld      $p, r3          # r3 = address of p
        ld      (r3), r4        # r4 = p
        add     r4, r2          # r2 = p + 12
        st      r1, (r2)        # *(p + 3) = b[0]

        halt

.pos 0x1000
a:      .long   0               # a
p:      .long   0               # p
b:      .long   0               # b[0]
        .long   1               # b[1]
        .long   2               # b[2]
        .long   3               # b[3]
        .long   4               # b[4]

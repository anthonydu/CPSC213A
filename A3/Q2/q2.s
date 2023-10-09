.pos 0x100
        # tmp = 0
        # tos = 0
        ld      $0, r0          # r0 = 0
        ld      $tmp, r1        # r1 = address of tmp
        st      r0, (r1)        # tmp = 0
        ld      $tos, r1        # r1 = address of tos
        st      r0, (r1)        # tos = 0

        # s[tos] = a[0]
        # tos++
        ld      $a, r0          # r0 = address of a
        ld      $s, r1          # r1 = address of s
        ld      0(r0), r2       # r2 = a[0]
        ld      $tos, r3        # r3 = address of tos
        ld      (r3), r4        # r4 = tos
        shl     $2, r4          # r4 = tos * 4
        add     r4, r1          # r1 = address of s[tos]
        st      r2, (r1)        # s[tos] = a[0]
        shr     $2, r4          # r4 = tos
        inc     r4              # r4 = tos + 1
        st      r4, (r3)        # tos++

        # s[tos] = a[1]
        # tos++
        ld      $a, r0          # r0 = address of a
        ld      $s, r1          # r1 = address of s
        ld      4(r0), r2       # r2 = a[1]
        ld      $tos, r3        # r3 = address of tos
        ld      (r3), r4        # r4 = tos
        shl     $2, r4          # r4 = tos * 4
        add     r4, r1          # r1 = address of s[tos]
        st      r2, (r1)        # s[tos] = a[1]
        shr     $2, r4          # r4 = tos
        inc     r4              # r4 = tos + 1
        st      r4, (r3)        # tos++

        # s[tos] = a[2]
        # tos++
        ld      $a, r0          # r0 = address of a
        ld      $s, r1          # r1 = address of s
        ld      8(r0), r2       # r2 = a[2]
        ld      $tos, r3        # r3 = address of tos
        ld      (r3), r4        # r4 = tos
        shl     $2, r4          # r4 = tos * 4
        add     r4, r1          # r1 = address of s[tos]
        st      r2, (r1)        # s[tos] = a[2]
        shr     $2, r4          # r4 = tos
        inc     r4              # r4 = tos + 1
        st      r4, (r3)        # tos++

        # tos--
        # tmp = s[tos]
        ld      $tos, r0        # r0 = address of tos
        ld      (r0), r1        # r1 = tos
        dec     r1              # r1 = tos - 1
        st      r1, (r0)        # tos--
        ld      $s, r2          # r1 = address of s
        ld      (r2, r1, 4), r2 # r2 = s[tos]
        ld      $tmp, r3        # r3 = address of tmp
        st      r2, (r3)        # tmp = s[tos]

        # tos--
        # tmp = tmp + s[tos]
        ld      $tos, r0        # r0 = address of tos
        ld      (r0), r1        # r1 = tos
        dec     r1              # r1 = tos - 1
        st      r1, (r0)        # tos--
        ld      $s, r2          # r1 = address of s
        ld      (r2, r1, 4), r2 # r2 = s[tos]
        ld      $tmp, r3        # r3 = address of tmp
        ld      (r3), r4        # r4 = tmp
        add     r4, r2          # r2 = tmp + s[tos]
        st      r2, (r3)        # tmp = tmp + s[tos]

        # tos--
        # tmp = tmp + s[tos]
        ld      $tos, r0        # r0 = address of tos
        ld      (r0), r1        # r1 = tos
        dec     r1              # r1 = tos - 1
        st      r1, (r0)        # tos--
        ld      $s, r2          # r1 = address of s
        ld      (r2, r1, 4), r2 # r2 = s[tos]
        ld      $tmp, r3        # r3 = address of tmp
        ld      (r3), r4        # r4 = tmp
        add     r4, r2          # r2 = tmp + s[tos]
        st      r2, (r3)        # tmp = tmp + s[tos]

        halt

.pos 0x1000
a:      .long   99              # a[0]
        .long   88              # a[1]
        .long   77              # a[2]
s:      .long   0               # s[0]
        .long   0               # s[1]
        .long   0               # s[2]
        .long   0               # s[3]
        .long   0               # s[4]
tos:    .long   0               # tos
tmp:    .long   0               # tmp
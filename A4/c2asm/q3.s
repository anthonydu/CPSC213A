.pos 0x1000
code:
        # v0 = s.x[i];
        ld $s, r0               # r0 = &s
        ld $i, r1               # r1 = &i
        ld (r1), r1             # r1 = i
        ld (r0, r1, 4), r2      # r2 = s.x[i]
        ld $v0, r3              # r3 = &v0
        st r2, (r3)

        # v1 = s.y[i];
        ld $s, r0               # r0 = &s
        ld $i, r1               # r1 = &i
        ld (r1), r1             # r1 = i
        ld 8(r0), r2            # r2 = s.y
        ld (r2, r1, 4), r3      # r3 = s.y[i]
        ld $v1, r4              # r3 = &v1
        st r3, (r4)

        # v2 = s.z->x[i];
        ld $s, r0               # r0 = &s
        ld $i, r1               # r1 = &i
        ld (r1), r1             # r1 = i
        ld 12(r0), r2           # r2 = s.z
        ld (r2, r1, 4), r3      # r3 = s.z->x[i]
        ld $v2, r4              # r3 = &v2
        st r3, (r4)

        # v3 = s.z->z->y[i];
        ld $s, r0               # r0 = &s
        ld $i, r1               # r1 = &i
        ld (r1), r1             # r1 = i
        ld 12(r0), r2           # r2 = s.z
        ld 12(r2), r3           # r3 = s.z->z
        ld 8(r3), r4            # r4 = s.z->z->y
        ld (r4, r1, 4), r5      # r5 = s.z->z->y[i]
        ld $v3, r6              # r6 = &v3
        st r5, (r6)

        halt

.pos 0x2000
static:
i:      .long 0

v0:     .long 0
v1:     .long 0
v2:     .long 0
v3:     .long 0

s:      .long 0                 # x[0]
        .long 0                 # x[1]
        .long sy                # s.y
        .long sz                # s.z

.pos 0x3000
heap:
sy:     .long 0                 # s.y[0]
        .long 0                 # s.y[1]

sz:     .long 0                 # s.z->x[0]
        .long 0                 # s.z->x[1]
        .long 0                 # s.z->y
        .long szz               # s.z->z

szz:    .long 0                 # s.z->z->x[0]
        .long 0                 # s.z->z->x[1]
        .long szzy              # s.z->z->y
        .long 0                 # s.z->z->z

szzy:   .long 0                 # s.z->z->y[0]
        .long 0                 # s.z->z->y[1]
.pos 0x100
swap:   ld      $array, r0      # r0: &array
        ld      12(r0), r3      # r3: array[3]
        ld      16(r0), r4      # r4: array[4]
        st      r3, 16(r0)      # array[4] => array[3]
        st      r4, 12(r0)      # array[3] => array[4]
        halt
.pos 0x1000
array:  .long   0xffffffff
        .long   0xffffffff
        .long   0xffffffff
        .long   0x12345678
        .long   0x87654321
        .long   0xffffffff
        .long   0xffffffff
        

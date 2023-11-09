.pos 0x1000
  ld $1, r0   # r0 = fd = 1 (standard out)
  ld $str, r1 # r1 = buffer 
  ld $12, r2  # r2 = length of string
  sys $1      # system call: write
  halt

.pos 0x2000
str:
  .long 0x68656c6c # hell
  .long 0x6f20776f # o wo
  .long 0x726c640a # rld\n

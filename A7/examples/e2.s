.pos 0x1000
  ld $0, r0   # r0 = fd = 0 (standard in)
  ld $buf, r1 # r1 = buf
  ld $16, r2  # r2 = size
  sys $0      # system call: read

  ld $1, r0   # r0 = fd = 1 (standard out)
  ld $buf, r1 # r1 = buf
  ld $16, r2  # r2 = size
  sys $1      # system call: write
  halt

.pos 0x2000
buf:
  .long 0
  .long 0
  .long 0
  .long 0

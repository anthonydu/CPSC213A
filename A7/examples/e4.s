.pos 0x1000
  ld $0, r0   # r0 = fd = 0 (standard in)
  ld $buf, r1 # r1 = buf
  ld $64, r2  # r2 = size
  sys $0      # system call: read

  mov r0, r1  # r1 = size = read return value
  ld $buf, r0 # r0 = buf
  sys $2      # system call: exec
  halt

.pos 0x2000
buf:
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0

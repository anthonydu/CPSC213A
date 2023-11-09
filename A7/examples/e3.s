.pos 0x1000
start:
  ld $stacktop, r5
  gpc $6, r6
  j main
  halt

main:
  ld $-16, r0
  add r0, r5

  ld $0, r0   # r0 = fd = 0 (standard in)
  mov r5, r1  # r1 = buf (on stack)
  ld $16, r2  # r2 = size
  sys $0      # system call: read

  ld $1, r0   # r0 = fd = 1 (standard out)
  mov r5, r1  # r1 = buf (on stack)
  ld $16, r2  # r2 = size
  sys $1      # system call: write

  ld $16, r0
  add r0, r5
  j (r6)

.pos 0x3000
stack:
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
stacktop:
  .long 0

.pos 0x100

# Compute all students' averages (1-2)

# setup loop
        ld $s, r0               # r0 = &s
        ld (r0), r0             # r0 = s
        ld $n, r1               # r1 = &n
        ld (r1), r1             # r1 = n
        not r1                  # r1 = ~n
        inc r1                  # r1 = -n
        # r1 is the looping condition
        ld $4, r2               # r2 = 4
        add r0, r2              # r2 = &grade
        # pointer to each student's grades

# looping condition
loop1:  beq r1, break1          # if r1 == 0 break

# calculate average for one student
        ld $0, r3               # r0 = sum = 0
        ld 0(r2), r4            # r2 = grade[0]
        add r4, r3              # r0 = sum += grade[0]
        ld 4(r2), r4            # r2 = grade[1]
        add r4, r3              # r0 = sum += grade[1]
        ld 8(r2), r4            # r2 = grade[2]
        add r4, r3              # r0 = sum += grade[2]
        ld 12(r2), r4           # r2 = grade[3]
        add r4, r3              # r0 = sum += grade[3]
        shr $2, r3              # r0 = avg = sum / 4
        st r3, 16(r2)           # average = avg

# increment loop
        ld $24, r3
        add r3, r2              # r2 += 24
        inc r1                  # r1 += 1
        br loop1                # continue









# Sorting the students (3-6)

# setup loop
break1: ld $s, r0               # r0 = &s
        ld (r0), r0             # r0 = s
        ld $n, r1               # r1 = &n
        ld (r1), r1             # r1 = n
        not r1                  # r1 = ~n
        inc r1                  # r1 = -n
        # r1 is the looping condition

# looping condition
loop2:  beq r1, break2          # if r1 == 0 break

# inner loop setup
        mov r0, r2              # r2 = &s1
        # a pointer to the first student
        ld $24, r3              # r3 = 40
        add r0, r3              # r2 = &s2
        # a pointer to the second student
        mov r1, r4              # r4 = r1
        inc r4                  # r4 = r1 + 1
        # the inner loop runs one less time than the size of the unsorted loop

# inner looping condition
loop3:  beq r4, break3          # if r3 == 0 break

# compare averages and maybe swap
        ld 20(r2), r5           # r5 = avg1
        not r5                  # r5 = ~avg1
        inc r5                  # r5 = -avg1
        ld 20(r3), r6           # r6 = avg2
        add r6, r5              # r6 = avg2 - avg1
        bgt r5, noswap          # if avg1 < avg2 noswap
        ld 20(r2), r5           # r5 = avg1
        st r5, 20(r3)           # avg2 = avg1
        st r6, 20(r2)           # avg1 = avg2
        ld (r2), r5             # r5 = sid1
        ld (r3), r6             # r6 = sid2
        st r5, (r3)             # sid2 = sid1
        st r6, (r2)             # sid1 = sid2

# increment inner loop
noswap: ld $24, r5
        add r5, r2              # r2 += 24
        add r5, r3              # r3 += 24
        inc r4                  # r4 += 1
        br loop3                # continue



# increment loop
break3: inc r1                  # r1 += 1
        br loop2                # continue

# students sorted, find median (7)
break2: ld $s, r0               # r0 = &s
        ld $n, r1               # r1 = &n
        ld (r1), r1             # r1 = n
        shr $1, r1              # r1 = n / 2
        inc r1                  # r1 = n / 2 + 1
        mov r1, r2              # r2 = n / 2 + 1
        shl $4, r1              # r1 = (n / 2 + 1) * 16
        shl $3, r2              # r2 = (n / 2 + 1) * 8
        add r2, r1              # r1 += r2
        ld (r0, r1, 4), r0      # r0 = median average sid
        ld $m, r1               # r1 = &m
        st r0, (r1)             # m = median average sid

        halt












# memory
.pos 0x1000
n:      .long 5         # number of students
m:      .long 0         # id of student with median average grade
s:      .long s0        # pointer to a dynamic array of students

s0:     .long 0         # sid
        .long 80        # grade[0]
        .long 60        # grade[1]
        .long 78        # grade[2]
        .long 90        # grade[3]
        .long 0         # average  

s1:     .long 1         # sid
        .long 90        # grade[0]
        .long 56        # grade[1]
        .long 77        # grade[2]
        .long 89        # grade[3]
        .long 0         # average 

s2:     .long 2         # sid
        .long 45        # grade[0]
        .long 32        # grade[1]
        .long 77        # grade[2]
        .long 61        # grade[3]
        .long 0         # average 

s3:     .long 3         # sid
        .long 37        # grade[0]
        .long 26        # grade[1]
        .long 45        # grade[2]
        .long 33        # grade[3]
        .long 0         # average 

s4:     .long 4         # sid
        .long 58        # grade[0]
        .long 89        # grade[1]
        .long 97        # grade[2]
        .long 76        # grade[3]
        .long 0         # average 
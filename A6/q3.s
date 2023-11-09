.pos 0x100

# Compute all students' averages (1-2)

# setup loop
        # r0: loop1 counter
        ld $n, r0               # r0 = &n
        ld (r0), r0             # r0 = n
        not r0                  # r0 = ~n
        inc r0                  # r0 = -n
        # r1: pointer to s
        ld $s, r1               # r1 = &s
        ld (r1), r1             # r1 = s
        

# looping condition
loop1:  beq r0, break1          # if r0 == 0 break

# calculate average for one student
        ld $0, r2               # r2 = sum = 0
        ld 4(r1), r3            # r3 = grade[0]
        add r3, r2              # sum += grade[0]
        ld 8(r1), r3            # r3 = grade[1]
        add r3, r2              # sum += grade[1]
        ld 12(r1), r3           # r3 = grade[2]
        add r3, r2              # sum += grade[2]
        ld 16(r1), r3           # r3 = grade[3]
        add r3, r2              # sum += grade[3]
        shr $2, r2              # r2 = avg = sum / 4
        st r2, 20(r1)           # average = avg

# increment loop
        ld $24, r2
        add r2, r1              # r1 += 24
        inc r0                  # r0 += 1
        br loop1                # continue









# Sorting the students (3-6)

# setup loop
        # r0: loop2 counter
break1: ld $n, r0               # r0 = &n
        ld (r0), r0             # r0 = n
        not r0                  # r0 = ~n
        inc r0                  # r0 = -n

# looping condition
loop2:  beq r0, break2          # if r0 == 0 break

# inner loop setup
        # a pointer to the first student
        ld $s, r1               # r1 = &s
        # a pointer to the next student
        ld $24, r2
        add r1, r2              # r2 = &s + 24
        # loop3 counter
        # the inner loop runs one less time than the size of the unsorted loop
        mov r0, r3              # r3 = r0
        inc r3                  # r3 = r0 + 1
        

# inner looping condition
loop3:  beq r3, break3          # if r3 == 0 break

# compare averages and maybe swap
        ld 20(r1), r4           # r4 = avg1
        not r4                  # r4 = ~avg1
        inc r4                  # r4 = -avg1
        ld 20(r2), r5           # r5 = avg2
        add r4, r5              # r4 = - avg1 + avg2 
        bgt r5, noswap          # if avg1 < avg2 noswap
        # swap sid
        ld (r1), r4             
        ld (r2), r5             
        st r4, (r2)             
        st r5, (r1)             
        # swap grade[0]
        ld 4(r1), r4             
        ld 4(r2), r5             
        st r4, 4(r2)             
        st r5, 4(r1)             
        # swap grade[1]
        ld 8(r1), r4             
        ld 8(r2), r5             
        st r4, 8(r2)             
        st r5, 8(r1)             
        # swap grade[2]
        ld 12(r1), r4             
        ld 12(r2), r5             
        st r4, 12(r2)             
        st r5, 12(r1)             
        # swap grade[3]
        ld 16(r1), r4             
        ld 16(r2), r5             
        st r4, 16(r2)             
        st r5, 16(r1)    
        # swap avg
        ld 20(r1), r4
        ld 20(r2), r5
        st r4, 20(r2)
        st r5, 20(r1)       

# increment inner loop
noswap: ld $24, r4
        add r4, r1              # r1 += 24
        add r4, r2              # r2 += 24
        inc r3                  # r4 += 1
        br loop3                # continue

# increment outer loop
break3: inc r0                  # r0 += 1
        br loop2                # continue








# students sorted, find median (7)
break2: ld $s, r0               # r0 = &s
        ld (r0), r0             # r0 = s
        ld $n, r1               # r1 = &n
        ld (r1), r1             # r1 = n
        shr $1, r1              # r1 = n / 2
        mov r1, r2              # r2 = n / 2
        shl $1, r1              # r1 = (n / 2) * 2
        shl $2, r2              # r2 = (n / 2) * 4
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

s0:     .long 1000      # sid
        .long 80        # grade[0]
        .long 60        # grade[1]
        .long 78        # grade[2]
        .long 90        # grade[3]
        .long 0         # average  

s1:     .long 1001      # sid
        .long 90        # grade[0]
        .long 56        # grade[1]
        .long 77        # grade[2]
        .long 89        # grade[3]
        .long 0         # average 

s2:     .long 1002      # sid
        .long 45        # grade[0]
        .long 32        # grade[1]
        .long 77        # grade[2]
        .long 61        # grade[3]
        .long 0         # average 

s3:     .long 1003      # sid
        .long 37        # grade[0]
        .long 26        # grade[1]
        .long 45        # grade[2]
        .long 33        # grade[3]
        .long 0         # average 

s4:     .long 1004      # sid
        .long 58        # grade[0]
        .long 89        # grade[1]
        .long 97        # grade[2]
        .long 76        # grade[3]
        .long 0         # average 
        lw      0     1     six
loop    lw      0     2     three
        noop
        noop
        add     1     2     4
        add     2     2     2
        add     4     4     4
        sw      0     1     1
        sw      0     2     three
        noop
        halt
six    .fill    6
three  .fill    3
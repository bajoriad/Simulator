        lw      0       1       6    load reg1 with 6 (symbolic address)
        lw      1       2       -3        load reg2 with -1 (nueric address)
start   ad     1       1       4
        nor     4       5       6    nor function check 
loop    jalr    4       3            checking 
        beq     5       2       1
        beq     2       4       si 
        noop
        halt 
size    .fill   7   
jmp main







game:   var #1
row:    var #1
player: var #1
board:  var #42

directions: var #8
static directions + #0, #1
static directions + #1, #1

static directions + #2, #1
static directions + #3, #0

static directions + #4, #0
static directions + #5, #1

static directions + #6, #65535  ; -1 em complemento de 2 (16 bits)
static directions + #7, #1

main:
    call setup

    __main__loop:
        call loop
        jmp __main__loop

setup:
    push r4

    loadn r0, #0
    loadn r1, #42
    loadn r2, #board
    loadn r4, #'_'

    __setup__loop_fill:
        cmp r0, r1
        jeg __setup__endloop_fill

        add r3, r2, r0  ; r3 = board + r0
        storei r3, r4   ; *(board + r0) = r4
        inc r0

    __setup__endloop_fill:

    pop r4
    rts

loop:
    call display_board
    rts

outchar_ij:
    loadn r3, #40
    mul r0, r0, r3
    add r0, r0, r1
    outchar r2, r0
    rts

outchar_ij_offset:
    loadn r3, #11
    add r0, r0, r3

    loadn r3, #16
    add r1, r1, r3

    call outchar_ij
    rts

; r0 = i, r1 = j
get_board_idx_ij:
    loadn r3, #6
    mul r0, r0, r3
    add r0, r0, r1
    loadn r3, #board
    add r0, r0, r3
    rts

display_board:
    push r4
    push r5

    loadn r4, #0    ; i = 0
    __display_board__for_i:
        loadn r0, #6
        cmp r4, r0
        jeg __display_board__endfor_i

        loadn r5, #0    ; j = 0
        __display_board__for_j:
            loadn r0, #7
            cmp r5, r0
            jeg __display_board__endfor_j

            mov r0, r4
            mov r1, r5
            call get_board_idx_ij   ; r0 = &board[i][j]
            loadi r0, r0            ; r0 = board[i][j]

            loadn r2, #2048
            loadn r3, #'_'
            
            loadn r1, #'2'
            cmp r0, r1
            jne __display_board__else_current_eq_2

            __display_board__if_current_eq_2:
                loadn r2, #3072
                loadn r3, #'2'
                jmp __display_board__endif_current_eq_2

            __display_board__else_current_eq_2:
                loadn r1, #'1'
                cmp r0, r1
                jne __display_board__endif_current_eq_1

                __display_board__if_current_eq_1:
                    loadn r3, #'1'
                    loadn r2, #2304

                __display_board__endif_current_eq_1:

            __display_board__endif_current_eq_2:

            ; color está correta
            mov r0, r4
            mov r1, r5
            add r2, r2, r3
            call outchar_ij_offset

            inc r5
            jmp __display_board__for_j

        __display_board__endfor_j:
        inc r4
        jmp __display_board__for_i

    __display_board__endfor_i:

    loadn r4, #0    ; i = 0
    __display_board__for_i_2:
        loadn r0, #7
        cmp r4, r0
        jeg __display_board__endfor_i_2

        loadn r0, #6
        mov r1, r4
        loadn r2, #'1'
        add r2, r2, r4
        call outchar_ij_offset

        inc r4
        jmp __display_board__for_i_2

    __display_board__endfor_i_2:

    pop r5
    pop r4
    rts


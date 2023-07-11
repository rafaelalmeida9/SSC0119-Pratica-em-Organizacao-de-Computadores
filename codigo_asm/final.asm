jmp main







game:   var #1
static game + #0, #1

player: var #1
static player + #0, #'1'

number_valid_moves: var #1
static number_valid_moves + #0, #0

board:  var #42

positions_x : var #4
positions_y : var #4

directions: var #8
static directions + #0, #1
static directions + #1, #1

static directions + #2, #1
static directions + #3, #0

static directions + #4, #0
static directions + #5, #1

static directions + #6, #65535  ; -1 em complemento de 2 (16 bits)
static directions + #7, #1

victory_msg_1: string "Voce venceu, jogador #1."
victory_msg_2: string "Voce venceu, jogador #2."
draw_msg: string "Ocorreu empate."

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

        jmp __setup__loop_fill

    __setup__endloop_fill:

    pop r4
    rts

loop:
    push r4
    push r5

    loadn r0, #game
    loadi r0, r0
    loadn r1, #0
    cmp r0, r1
    jne __loop__game_is_running

    halt

    __loop__game_is_running:

    inchar r0
    loadn r1, #0 ; wincode
    loadn r2, #255
    cmp r0, r2
    jeq __loop__final_part
    loadn r2, #'1'
    cmp r0, r2
    jle __loop__final_part
    loadn r2, #'0'
    loadn r3, #7
    add r2, r2, r3
    cmp r0, r2
    jgr __loop__final_part

    loadn r2, #'1'
    sub r0, r0, r2
    call place_object
    call verify_winner
    mov r4, r0
    mov r5, r1
    call toggle_player 

    __loop__final_part:
    call display_board
    
    loadn r0, #game
    loadi r0, r0
    loadn r1, #0
    cmp r0, r1
    jne __loop__final_final_part

    ; o jogo acabou
    loadn r0, #0
    cmp r4, r0
    jeq __loop__draw

    ; nao ocorreu empate
    cmp r5, r0
    jeq __loop__vitoria_jogador_1

    ; nesse caso o jogador 2 ganhou
    __loop__vitoria_jogador_2:
    loadn r0, #victory_msg_2
    loadn r1, #512
    jmp __loop__final_final_part

    __loop__vitoria_jogador_1:
    loadn r0, #victory_msg_1
    loadn r1, #512
    jmp __loop__final_final_part

    __loop__draw:
    loadn r0, #draw_msg
    loadn r1, #2816
    call display_centralized_msg

    __loop__final_final_part:
    pop r5
    pop r4
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
    loadn r3, #7
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


display_msg:

    push r4
    push r5

    mov r4, r2  ; r4 = msg
    mov r5, r3  ; r5 = cor

    __display_msg__loop:
        loadi r2, r4  ; r2 = *msg
        loadn r3, #0  ; r3 = 0
        cmp r2, r3    ; == '0'
        jeq __display_msg__endloop

        push r0
        push r1
        add r2, r2, r5  ; r2 = (*msg) + color
        call outchar_ij ; outchar_ij(r0, r1, r2)
        pop r1
        pop r0

        inc r1

        loadn r3, #40
        cmp r1, r3
        jne __display_msg__endif_r1_eq_screen_res

        __display_msg__if_r1_eq_screen_res:
            loadn r1, #0
            inc r0
        
        __display_msg__endif_r1_eq_screen_res:
        inc r4
        jmp __display_msg__loop

    __display_msg__endloop:

    pop r5
    pop r4

    rts

; r0 = msg
; returns r0 = len
strlen:

    mov r1, r0 ; r1 = str
    loadn r0, #0; len = 0

    __strlen__loop:
        loadi r2, r1 ; r2 = *str
        loadn r3, #0
        cmp r2, r3 ; r2 = '0'
        jeq __strlen__endloop

        inc r1 ; str++
        inc r0 ; len++

        jmp __strlen__loop

    __strlen__endloop:

    rts

; r0 = msg
; r1 = color
display_centralized_msg:

    push r1
    push r4

    mov r2, r0 ; r2 = msg

    loadn r1, #16 ; r1 = offsetx

    loadn r0, #11
    loadn r3, #2
    sub r0, r0, r3 ; r0 = offsety-2

    push r0
    push r1
    push r2
    mov r0, r2 ; r0 = msg
    call strlen ; r0 = length(msg)
    mov r3, r0; r3 = length(msg)
    pop r2
    pop r1
    pop r0

    loadn r4, #2
    div r3, r3, r4
    sub r1, r1, r3 ; r1 = offsetx - length(msg)/2

    loadn r4, #3
    add r1, r1, r4 ; r1 = offsetx - length(msg)/2 + 3

    pop r4
    pop r3 ; r3 = color

    call display_msg

    rts

; swaps r0 and r1 modifying r2
swap_first_two:
    mov r2, r0
    mov r0, r1
    mov r1, r2
    rts

; r0 = column number
place_object:
    
    push r4
    
    loadn r1, #0; row

    __place_object__loop:

        loadn r4, #6
        cmp r1, r4
        jeq __place_object__endloop

        push r0
        push r1
        call swap_first_two
        call get_board_idx_ij
        mov r3, r0 ; r3 = &board[i][j]
        pop r1
        pop r0
        loadi r3, r3; r3 = board[i][j]

        loadn r4, #'_'
        cmp r3, r4
        jne __place_object__endloop

        inc r1
        jmp __place_object__loop
    
    __place_object__endloop:

    loadn r4, #0
    cmp r1, r4
    jeq __place_object_do_nothing

    loadn r4, #1 
    sub r1, r1, r4
    call swap_first_two
    call get_board_idx_ij; r0 = &board[i][j]
    loadn r4, #player
    loadi r4, r4
    storei r0, r4; board[i][j] = player

    __place_object_do_nothing:

    pop r4

    rts

; r0 = i, r1 = j
; return = r0 (1 or 0)
inside_board:

    loadn r3, #0

    loadn r2, #0
    cmp r0, r2
    jle __inside_board__end

    loadn r2, #6
    cmp r0, r2
    jeg __inside_board__end

    loadn r2, #0
    cmp r1, r2
    jle __inside_board__end

    loadn r2, #7
    cmp r1, r2
    jeg __inside_board__end

    loadn r3, #1

    __inside_board__end:
    mov r0, r3
    
    rts

; r0=i, r1=j, r2=int* d_is, r3 = int* d_js
; return r0 = 0 se nao ha winner, r0=1 se ha.
; return r1 (em caso de winner) = 0 ou 1 (1 ou 2 resp.)
check_win_direction_pos:
    push r4
    push r5
    push r6
    push r7

    push r0
    push r3
    call get_board_idx_ij; r0 = &board[i][j]
    loadi r4, r0 ; r4 = board[i][j]
    pop r3
    pop r0

    loadn r5, #'_'
    cmp r4, r5
    jeq __check_win_direction_pos__ret_0

    loadn r5, #0 ; k=1

    __check_win_direction_pos__for:
        loadn r6, #4
        cmp r5, r6
        jeq __check_win_direction_pos__endfor

        add r6, r2, r5 ; r6 = (d_is + k)
        loadi r6, r6 ; r6 = d_is[k]
        add r6, r0, r6 ; r6 = d_is[k] + i

        add r7, r3, r5 ; r7 = (d_js + k)
        loadi r7, r7 ; r7 = d_js[k]
        add r7, r1, r7 ; r7 = d_js[k] + j
        push r5

        push r0
        push r3
        push r2
        mov r0, r6
        mov r1, r7
        call inside_board ; r0 = 0 if not inside
        mov r5, r0 ; r5 = retorno
        pop r2
        pop r3
        pop r0

        ; se r5 = 0, retorna 0
        push r0
        loadn r0, #0
        cmp r0, r5 ; se r5 = 0
        pop r0 
        pop r5
        jeq __check_win_direction_pos__ret_0

        ; se board[new_i][new_j] != r4, abortar
        push r0
        push r3
        mov r0, r6
        mov r1, r7
        call get_board_idx_ij; r0 = &board[new_i][new_j]
        loadi r6, r0 ; r6 = board[new_i][new_j]
        pop r3
        pop r0
        cmp r4, r6
        jne __check_win_direction_pos__ret_0

        inc r5

    __check_win_direction_pos__endfor: 
    
    jmp __check_win_direction_pos__ret_1


    __check_win_direction_pos__ret_0:
    loadn r0, #0
    loadn r1, #0
    
    jmp __check_win_direction_pos__end


    __check_win_direction_pos__ret_1:
    loadn r0, #1
    mov r3, r4
    loadn r2, #'1'
    sub r1, r3, r2

    __check_win_direction_pos__end:
    pop r7
    pop r6
    pop r5
    pop r4

    rts

; r0 = int* d_is, r1 = int* d_js
; returns like the previous function
check_win_direction_board:
    push r4
    push r5
    push r6
    push r7

    loadn r2, #0 ; did_i_win = 0

    loadn r3, #0 ; i = 0
    __check_win_direction_board__for_i:
        loadn r4, #6
        cmp r3, r4
        jeq __check_win_direction_board__endfor_i

        loadn r4, #0 ; j = 0
        __check_win_direction_board__for_j:
            loadn r5, #7
            cmp r4, r5
            jeq __check_win_direction_board__endfor_j


            push r0
            push r1
            push r2
            push r3
            
            ; (r0, r1, r2, r3) = (r3, r4, r0, r1)
            mov r6, r0
            mov r7, r1
            mov r0, r3
            mov r1, r4
            mov r2, r6 ; = r0
            mov r3, r7 ; = r1

            call check_win_direction_pos
            mov r6, r0
            loadn r7, #0
            cmp r6, r7
            mov r7, r1

            pop r3
            pop r2
            pop r1
            pop r0

            jne __check_win_direction_board__ret_1

            inc r4
        __check_win_direction_board__endfor_j:

        inc r3

    __check_win_direction_board__endfor_i:


    __check_win_direction_board__ret_0:
    loadn r0, #0
    jmp __check_win_direction_board__end


    __check_win_direction_board__ret_1:
    loadn r0, #1
    mov r1, r7


    __check_win_direction_board__end:
    pop r7
    pop r6
    pop r5
    pop r4

    rts

; r0 = int* pos_xs, r1 = int* pos_ys, r2 = dx, r3 = dy
directionize:
    push r4
    push r5
    push r6

    loadn r4, #0 ; 'i'
    __directionize__loop:
        loadn r5, #4
        cmp r4, r5
        jeq __directionize__endloop

        add r5, r0, r4 ; pos_xs + i
        mul r6, r2, r4 ; dx * i
        storei r5, r6 ; pos_xs[i] = dx * i

        add r5, r1, r4 ; pos_ys + i
        mul r6, r3, r4 ; dy * i
        storei r5, r6 ; pos_ys[i] = dy * i

        inc r4
    __directionize__endloop:

    pop r6
    pop r5
    pop r4

    rts

; r0 = dx, r1 = dy
; retorna como a funcao acima
check_win_direction:

    push r4
    push r5
    push r6 

    loadn r2, #positions_x
    loadn r3, #positions_y

    push r0
    push r1
    push r2
    push r3
    ; (r0, r1, r2, r3) = (r2, r3, r0, r1)
    mov r4, r0 
    mov r0, r2
    mov r5, r1
    mov r1, r3
    mov r2, r4 ; r0
    mov r3, r5 ; r1
    call check_win_direction_board
    mov r4, r0
    mov r5, r1 

    pop r3
    pop r2
    pop r1
    pop r0


    mov r0, r4
    mov r1, r5 

    pop r6
    pop r5
    pop r4

    rts

; returns r0 = &(directions[i][j])
get_direction_idx_ij:
    loadn r3, #4
    mul r0, r0, r3
    add r0, r0, r1
    loadn r3, #directions
    add r0, r0, r3
    rts


; returns r0, r1 according to functions above
verify_winner:

    push r4
    push r5

    loadn r0, #0
    loadn r1, #0
    loadn r2, #0 ; i=0

    __verify_winner__loop:
        loadn r3, #4
        cmp r2, r3
        jeq __verify_winner__endloop

        push r0
        push r1
        push r2
        push r3

        mov r0, r2 ; r0 = i
        loadn r1, #0 ; r1 = 0
        call get_direction_idx_ij ; r0 = &(directions[i][0])
        loadi r0, r0 ; r0 = directions[i][0]
        mov r4, r0 ; salva em r4

        mov r0, r2 ; r0 = i
        loadn r1, #1; r1 = 1
        call get_direction_idx_ij ; r0 = &(directions[i][1])
        loadi r1, r0 ; r1 = directions[i][1]

        mov r0, r4 ; r0 volta a ser directions[i][0]

        ; r0 e r1 agora estao corretos para poder chamar
        call check_win_direction 
        ; depois da chamada, temos o win code
        ; salvo em r4 e em r5
        mov r4, r0
        mov r5, r1

        loadn r0, #0
        cmp r4, r0

        pop r3
        pop r2
        pop r1
        pop r0

        jne __check_win_direction_board__ret_1

        inc r2
    __verify_winner__endloop:

    __verify_winner_ret_0:
    loadn r0, #0
    loadn r1, #0
    jmp __verify_winner__end

    __verify_winner_ret_1:
    mov r0, r4
    mov r1, r5
    loadn r4, #game ; game = 0
    loadn r5, #0
    storei r4, r5

    __verify_winner__end:
    pop r5
    pop r4

    rts


toggle_player:
    loadn r0, #player ; r0 = &player
    loadi r1, r0 ; r1 = player
    loadn r2, #'1' ; r2 = '1'
    sub r1, r1, r2 ; r1 = 0 se era '1', 1 se era '2'
    loadn r2, #1 ; r2 = 1
    sub r1, r2, r1 ; r1 = 1 se era 0, 0 se era 1
    loadn r2, #'1'
    add r1, r1, r2 ; r1 = '1' se era 0, '2' se era 1
    storei r0, r1 ; player = r1

    rts

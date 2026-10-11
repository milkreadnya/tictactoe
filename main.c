#include <stdio.h>
#include <string.h>
#include <stdbool.h>



unsigned g_prng_state = 0x4AF7484Bu;
typedef enum { ONGOING, PLAYER_WIN, BOT_WIN, DRAW } GameState;


void draw_board(unsigned size, unsigned state[size][size]){
    for(unsigned x = 0; x < size; x++){
        for(unsigned y = 0; y < size; y++){
            // вывод
            switch(state[x][y]){
                case 1:
                    printf("%c ", 'X');
                    break;
                case 2:
                    printf("%c ", '0');
                    break;
                default:
                    printf("%c ", '.');
            }

        }
        printf("\n");
    }
}

void clear_terminal(){
    printf("\033[H\033[2J");
}

int prng(unsigned size){
    g_prng_state = (0xB7BF48FFu * g_prng_state + 0x133B7101u);
    return (g_prng_state >> 16) % size;
}

int is_end(unsigned size, unsigned state[size][size]) {
    for (unsigned x = 0; x < size; x++) {
        unsigned first = state[x][0];
        if (first == 0) continue;
        
        int win = 1;
        for (unsigned y = 1; y < size; y++) {
            if (state[x][y] != first) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    for (unsigned y = 0; y < size; y++) {
        unsigned first = state[0][y];
        if (first == 0) continue;
        
        int win = 1;
        for (unsigned x = 1; x < size; x++) {
            if (state[x][y] != first) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    unsigned first = state[0][0];
    if (first != 0) {
        int win = 1;
        for (unsigned x = 1; x < size; x++) {
            if (state[x][x] != first) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    first = state[0][size - 1];
    if (first != 0) {
        int win = 1;
        for (unsigned x = 1; x < size; x++) {
            if (state[x][size - 1 - x] != first) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    return 0;
}

int is_draw(unsigned size, unsigned state[size][size]){
    for(unsigned x = 0; x < size; x++){
        for(unsigned y = 0; y < size; y++){
            if(state[x][y] == 0){
                return 0;
            }
        }
    }
    return 1;
}

void bot_move(unsigned size, unsigned state[size][size]) {
    unsigned free_count = 0;
    for (unsigned i = 0; i < size; i++)
        for (unsigned j = 0; j < size; j++)
            if (state[i][j] == 0) free_count++;
    if (free_count == 0) return;

    unsigned pick = prng(free_count);
    for (unsigned i = 0; i < size; i++)
        for (unsigned j = 0; j < size; j++)
            if (state[i][j] == 0 && pick-- == 0) {
                state[i][j] = 2;
                return;
            }
}

int main(){
    
    unsigned size = 0;
    bool ended = 0;


    clear_terminal();
    while (true) {
        puts("Enter size of the board (2-1000):");
        if (scanf("%u", &size) != 1 || size < 2 || size > 1000) {
            puts("Invalid size");
        } else break;
    }
    unsigned board_state[size][size];
    memset(board_state, 0, sizeof(board_state));

    while(!ended){
        clear_terminal();
        draw_board(size, board_state);

        unsigned user_x = 0, user_y = 0;
        puts("Enter you move coordinates (x y):");

        while(true){
            scanf("%u %u", &user_x, &user_y);

            if((user_x != 0 && user_y != 0) && (user_y <= size && user_x <= size)){ // Прием координат с единицы
                --user_y; --user_x;
                if(board_state[user_y][user_x] == 0){ // У тебя в двумерном массиве сначала выбор строки потом столбца, поэтому по сути стейт имеет вид state[y][x]
                    board_state[user_y][user_x] = 1;
                    break;
                }
                else{
                    puts("Wrong coordinates (entered grid cell already filled), enter again (x y):");
                }
            }
            else{
                puts("Wrong coordinates (out of grid), enter again (x y):");
            }
        }


        if (is_end(size, board_state)){
            ended = 1;
            clear_terminal();
            puts("You won!");
            break;
        }
        
        if (is_draw(size, board_state)){
            ended = 1;
            clear_terminal();
            puts("Draw!");
            break;
        }


        bot_move(size, board_state);
        if (is_end(size, board_state)){
            ended = 1;
            clear_terminal();
            puts("Bot won!");
            break;
        }

        if (is_draw(size, board_state)){
            ended = 1;
            clear_terminal();
            puts("Draw!");
            break;
        }

    }
        
    draw_board(size, board_state);
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


unsigned g_prng_state = 0x4AF7484Bu;


void draw_board(unsigned size, unsigned state[size][size]){
    for(unsigned y = 0; y < size; y++){
        for(unsigned x = 0; x < size; x++){
            switch(state[y][x]){
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
    fflush(stdout);
}

int prng(unsigned size){
    g_prng_state = (0xB7BF48FFu * g_prng_state + 0x133B7101u);
    return (g_prng_state >> 16) % size;
}

int is_end(unsigned size, unsigned state[size][size]) {
    for (unsigned y = 0; y < size; y++) {
        unsigned first = state[y][0];
        if (first == 0) continue;

        int win = 1;
        for (unsigned x = 1; x < size; x++) {
            if (state[y][x] != first) {
                win = 0;
                break;
            }
        }
        if (win) return first;
    }
    for (unsigned x = 0; x < size; x++) {
        unsigned first = state[0][x];
        if (first == 0) continue;

        int win = 1;
        for (unsigned y = 1; y < size; y++) {
            if (state[y][x] != first) {
                win = 0;
                break;
            }
        }
        if (win) return first;
    }
    unsigned first = state[0][0];
    if (first != 0) {
        int win = 1;
        for (unsigned i = 1; i < size; i++) {
            if (state[i][i] != first) {
                win = 0;
                break;
            }
        }
        if (win) return first;
    }
    first = state[0][size - 1];
    if (first != 0) {
        int win = 1;
        for (unsigned i = 1; i < size; i++) {
            if (state[i][size - 1 - i] != first) {
                win = 0;
                break;
            }
        }
        if (win) return first;
    }
    return 0;
}

int is_draw(unsigned size, unsigned state[size][size]){
    for(unsigned y = 0; y < size; y++){
        for(unsigned x = 0; x < size; x++){
            if(state[y][x] == 0){
                return 0;
            }
        }
    }
    return 1;
}

void bot_move(unsigned size, unsigned state[size][size]){
    while(true){
        unsigned x = prng(size);
        unsigned y = prng(size);
        if (state[y][x] == 0){
            state[y][x] = 2;
            break;
        }
    }
}

int main(){

    unsigned size = 0;
    bool ended = false;
    int mode = 0;

    clear_terminal();
    puts("Select game mode:");
    puts("1 - Player vs Player");
    puts("2 - Player vs Bot");
    while(true) {
        if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            puts("Wrong mode, please try again (1 or 2):");
        } else break;
    }

    clear_terminal();
    puts("Enter size of the board (2-10):");
    while(true) {
        if (scanf("%u", &size) != 1 || size < 2 || size > 10) {
            puts("Wrong size, please try again:");
        } else break;
    }

    unsigned board_state[size][size];

    memset(board_state, 0, sizeof(board_state));

    unsigned current_player = 1;

    while(!ended){
        clear_terminal();
        draw_board(size, board_state);

        unsigned user_x = 0, user_y = 0;

        if (mode == 1) {
            printf("Player %u (%c), enter your move coordinates (format:x y):\n",
                   current_player, current_player == 1 ? 'X' : '0');
        } else {
            puts("Enter you move coordinates (format:x y):");
        }

        while(true){
            if (scanf("%u %u", &user_x, &user_y) != 2) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                puts("Wrong input, please try again (format:x y):");
                continue;
            }

            if((user_y != 0 && user_x != 0) && (user_x <= size && user_y <= size)){
                user_x--;user_y--;
                if(board_state[user_y][user_x] == 0){
                    board_state[user_y][user_x] = current_player;
                    break;
                }
                else{
                    puts("Wrong coordinates (entered grid cell already filled), please try again (format:x y):");
                }
            }
            else{
                puts("Wrong coordinates (out of grid), please try again (format:x y):");
            }
        }

        int winner = is_end(size, board_state);
        if (winner){
            ended = true;
            clear_terminal();
            if (mode == 1) {
                printf("Player %d (%c) won!\n", winner, winner == 1 ? 'X' : '0');
            } else {
                puts(winner == 1 ? "You won!" : "Bot won!");
            }
            break;
        }

        if (is_draw(size, board_state)){
            ended = true;
            clear_terminal();
            puts("Draw!");
            break;
        }

        if (mode == 2) {
            bot_move(size, board_state);
            winner = is_end(size, board_state);
            if (winner){
                ended = true;
                clear_terminal();
                puts(winner == 1 ? "You won!" : "Bot won!");
                break;
            }

            if (is_draw(size, board_state)){
                ended = true;
                clear_terminal();
                puts("Draw!");
                break;
            }
        } else {
            current_player = (current_player == 1) ? 2 : 1;
        }
    }

    draw_board(size, board_state);
    return 0;
}
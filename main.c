#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


void draw_board(unsigned size, unsigned state[size][size]){
    printf("   ");
    for(unsigned col = 0; col < size; col++){
        printf("%-2u", col + 1);
    }
    putchar('\n');

    for(unsigned row = 0; row < size; row++){
        printf("%-2u ", size - row);

        for(unsigned col = 0; col < size; col++){
            switch(state[row][col]){
                case 1:
                    putchar('X');
                    putchar(' ');
                    break;
                case 2:
                    putchar('0');
                    putchar(' ');
                    break;
                default:
                    putchar('.');
                    putchar(' ');
            }
        }
        putchar('\n');
    }
}

void clear_terminal(){
    printf("\033[H\033[2J");
    fflush(stdout);
}

int prng(unsigned size){
    return rand() % size;
}

// 0 — нет конца, 1 — победил игрок, 2 — победил бот // вайбкод
int is_end(unsigned size, unsigned state[size][size]) {
    for (unsigned row = 0; row < size; row++) {
        unsigned first = state[row][0];
        if (first == 0) continue;

        int win = 1;
        for (unsigned col = 1; col < size; col++) {
            if (state[row][col] != first) {
                win = 0;
                break;
            }
        }
        if (win) return first;
    }
    for (unsigned col = 0; col < size; col++) {
        unsigned first = state[0][col];
        if (first == 0) continue;

        int win = 1;
        for (unsigned row = 1; row < size; row++) {
            if (state[row][col] != first) {
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
    for(unsigned row = 0; row < size; row++){
        for(unsigned col = 0; col < size; col++){
            if(state[row][col] == 0){
                return 0;
            }
        }
    }
    return 1;
}

void bot_move(unsigned size, unsigned state[size][size]){
    unsigned free_count = 0;
    for (unsigned row = 0; row < size; row++)
        for (unsigned col = 0; col < size; col++)
            if (state[row][col] == 0) free_count++;

    if (free_count == 0) return;

    unsigned pick = prng(free_count);
    for (unsigned row = 0; row < size; row++)
        for (unsigned col = 0; col < size; col++)
            if (state[row][col] == 0 && pick-- == 0) {
                state[row][col] = 2;
                return;
            }
}

int main(){

    srand(time(NULL));

    unsigned size = 0;
    bool ended = false;


    clear_terminal();
    puts("Enter size of the board (2-1000):");
    if (scanf("%u", &size) != 1 || size < 2 || size > 1000) {
        puts("Wrong size");
        return 1;
    }

    unsigned board_state[size][size];

    memset(board_state, 0, sizeof(board_state));

    while(!ended){
        clear_terminal();
        draw_board(size, board_state);

        unsigned user_x = 0, user_y = 0;
        puts("Enter you move coordinates (x y)");

        while(true){
            if (scanf("%u %u", &user_x, &user_y) != 2) { //Проверка на некорректный ввод
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                puts("Wrong input, enter again (x y):");
                continue;
            }

            if((user_y != 0 && user_x != 0) && (user_x <= size && user_y <= size)){ // Прием координат с единицы, y снизу вверх
                unsigned row = size - user_y; // инверсия: y=size — верх, y=1 — низ
                unsigned col = user_x - 1;
                if(board_state[row][col] == 0){
                    board_state[row][col] = 1;
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


        int winner = is_end(size, board_state);
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

    }


    draw_board(size, board_state);
    return 0;
}
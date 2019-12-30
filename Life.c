#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Life.h"
#include <unistd.h>
#include <termios.h>

struct l_cursor c;
_Bool pause_mode = 0;


int main(void) {
    srand(time(NULL));
    enable_raw_termios_settings();
    adjust_terminal(60, 201);
    grid_reset();
    WIPE;

    user_input = 0;
    do {

        display_main_grid();
        printf(KEYBINDS);

        user_input = getchar();

        if ( user_input == 'p'){
            pause_mode = 1;
            printf("\n\t\t\t\t\t\t\tPAUSED");
            c.x=50;
            c.y=25;
            do {
                user_input = getchar();
                WIPE;
                switch(user_input){
                    case 'w' : c.y = mod( (c.y)-1, GRID_HEIGHT );
                    break;

                    case 'a' : c.x = mod( (c.x)-1, GRID_WIDTH);
                    break;

                    case 'd' : c.x = mod( (c.x)+1, GRID_WIDTH);
                    break;

                    case 's' : c.y = mod( (c.y)+1, GRID_HEIGHT );
                    break;

                    case ' ' : {
                        if (grid_main[c.y][c.x] == 0) grid_main[c.y][c.x] = 1;
                        else grid_main[c.y][c.x] = 0;
                    }break;

                    case 'r' : grid_reset();
                    break;

                    case 't' :{
                        display_main_grid();
                        printf( KEYBINDS "\n\t\t\t\t\t\tEnter a value between 1-9 to determine fertility.");
                        randomize_grid(getchar());
                        WIPE;
                    }break;

                    case 'f' :{
                        display_main_grid();
                        printf( KEYBINDS "\n\t\t\t\t\t\t\tPress one of the keys -1 -2 -3 to save a grid.");
                        user_input = getchar();
                        save_game(user_input);

                    }break;

                    case 'g' :{
                        display_main_grid();
                        printf( KEYBINDS "\n\t\t\t\t\t\t\tPress one of the keys -1 -2 -3 to load a grid.");
                        user_input = getchar();
                        load_game(user_input);


                    }break;
                }

                display_main_grid();
                printf( KEYBINDS "\n\t\t\t\t\t\t\tPAUSED");
            }while(user_input != 'p');
            pause_mode = 0;
        }
        calculate();
        WIPE;

    }while(user_input != 'q');





    disable_raw_termios_settings();
    return 0;

}


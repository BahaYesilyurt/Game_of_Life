#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#define WIPE printf("\e[H\e[J")
#define GRID_HEIGHT 50
#define GRID_WIDTH 100
#define KEYBINDS "\n\tPress -p to pause and edit.\tMove cursor with -w -a -s -d keys.\t\t\tPress -space to turn cell on\\off\n\tPress -r to reset the grid.\tPress -q while unpaused to terminate the program.\tPress -t to generate random grid.\n\tPress -f to save the grid.\tPress -g to load a game."

char grid_main[GRID_HEIGHT][GRID_WIDTH];
char grid_memory[GRID_HEIGHT][GRID_WIDTH];
int user_input;
_Bool pause_mode;

struct l_cursor{
    int x, y;
}c;

struct termios default_termios;
struct termios raw_termios;
void disable_raw_termios_settings(void){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &default_termios);
}
void enable_raw_termios_settings(void){
    tcgetattr(STDIN_FILENO, &raw_termios);
    raw_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
}
void adjust_terminal(short height, short width){
    printf("\e[8;%hi;%hit", height, width);         //These are ANSI escape sequences which come packaged with c99. I merely looked them up in the internet.
    printf("\e[3;0;0t");                            //The one on the top adjusts terminal size and other one moves the window to top left corner.
}



int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

void calculate(){
    short neighbor_counter=0;
    for(int i=0; i<GRID_HEIGHT; i++){
        for(int j=0; j<GRID_WIDTH; j++){    //going through the grid.

            for(int k=-1; k<=1; k++){          //checking neighbors of the cell it's going through.
                for(int l=-1; l<=1; l++){
                    if( k == 0 && l == 0) continue;
                    if ( grid_main [ mod((i+k),GRID_HEIGHT) ] [ mod((j+l),GRID_WIDTH) ] == 1) neighbor_counter++;
                }
            }
            if(  ( (neighbor_counter == 2) && (grid_main[i][j] == 1) ) || (neighbor_counter == 3)  ) grid_memory[i][j] = 1;
            else grid_memory[i][j] = 0;
            neighbor_counter = 0;                         // If the cell has 2 neighbors it doesn't change states, if it has 3 it turns on and else it turns off.
        }
    }
    for(int i=0; i<GRID_HEIGHT; i++){
        for(int j=0; j<GRID_WIDTH; j++) grid_main[i][j] = grid_memory[i][j];
    }
}

void display_main_grid(void){
    for(int i=0; i < GRID_HEIGHT; i++){
        for(int j=0; j < GRID_WIDTH; j++) {

            if( (i == 0 && j == 0)) {for(int k=0; k<GRID_WIDTH*2+1; k++) printf("_"); printf("\n");}

            if( pause_mode == 1 && i == c.y && j == c.x ) printf(" \u2573");
            else {grid_main[i][j] ? printf("\u2B1C") : printf("  ");}

            if (j == GRID_WIDTH - 1) printf("|\n");
            if( (i == GRID_HEIGHT-1 && j == GRID_WIDTH-1)) for(int k=0; k<GRID_WIDTH*2+1; k++) printf("\u203E");

        }
    }
}

void grid_reset(void){
    for(int i=0; i < GRID_HEIGHT; i++){
        for(int j=0; j < GRID_WIDTH; j++) {
            grid_main[i][j] = 0;
            grid_memory[i][j] = 0;
        }
    }
}

void randomize_grid(char fertility){    //inspired by an online post, the function is self explanatory.
    grid_reset();
    if( fertility >= '1'  &&  fertility <='9') {
        for (short i = 0; i < GRID_HEIGHT; i++) {
            for (short j = 0; j < GRID_WIDTH; j++) {
                if( fertility-'0' > rand()%10) grid_main[i][j] = 1;
            }
        }
    }
    else  printf("\t\t\tINVALID INPUT\n");
}

void save_game(char save_to){

    switch(save_to){
        case '1':{
            FILE *save1 = fopen( "SAVE_1.txt", "w" );

            for(int i = 0; i < GRID_HEIGHT; ++i) {
                for(int j = 0; j < GRID_WIDTH; ++j) {
                    grid_main[i][j] ? fprintf(save1, "1") : fprintf(save1, "0");
                }
            }
            fclose(save1);
            WIPE;
        }break;
        case '2':{
            FILE *save2 = fopen( "SAVE_2.txt", "w" );

            for(int i = 0; i < GRID_HEIGHT; ++i) {
                for(int j = 0; j < GRID_WIDTH; ++j) {
                    grid_main[i][j] ? fprintf(save2, "1") : fprintf(save2, "0");
                }
            }
            fclose(save2);
            WIPE;
        }break;
        case '3':{
            FILE *save3 = fopen( "SAVE_3.txt", "w" );

            for(int i = 0; i < GRID_HEIGHT; ++i) {
                for(int j = 0; j < GRID_WIDTH; ++j) {
                    grid_main[i][j] ? fprintf(save3, "1") : fprintf(save3, "0");
                }
            }
            fclose(save3);
            WIPE;
        }break;
        default:{
            WIPE;
            display_main_grid();
            printf( KEYBINDS "\n\t\t\t\t\t\tERROR: FILE COULD NOT BE SAVED. Press any key to continue.");
            getchar();
            WIPE;

        }
    }
}

void load_game(char load_from){

    switch(load_from){
        case '1':{
            FILE *save1;
            char str[GRID_HEIGHT*GRID_WIDTH];
            if( (save1 = fopen("SAVE_1.txt","r"))){

                while( fgets(str, GRID_HEIGHT*GRID_WIDTH, save1) != NULL);
                short counter = 0;
                for(int i=0; i<GRID_HEIGHT; i++){
                    for(int j=0; j<GRID_WIDTH; j++){
                        if ( str[counter] == '0') grid_main[i][j] = 0;
                        else if( str[counter] == '1' ) grid_main[i][j] = 1;
                        counter++;
                    }
                }
            }
            else{
                WIPE;
                display_main_grid();
                printf( KEYBINDS "\n\t\t\t\t\t\tERROR: FILE COULD NOT BE OPENED. Press any key to continue.");
                getchar();
            }
            WIPE;
            fclose(save1);
        }break;
        case '2':{
            FILE *save2;
            char str[GRID_HEIGHT*GRID_WIDTH];
            if( (save2 = fopen("SAVE_2.txt","r"))){

                while( fgets(str, GRID_HEIGHT*GRID_WIDTH, save2) != NULL);
                short counter = 0;
                for(int i=0; i<GRID_HEIGHT; i++){
                    for(int j=0; j<GRID_WIDTH; j++){
                        if ( str[counter] == '0') grid_main[i][j] = 0;
                        else if( str[counter] == '1' ) grid_main[i][j] = 1;
                        counter++;
                    }
                }
            }
            else{
                WIPE;
                display_main_grid();
                printf( KEYBINDS "\n\t\t\t\t\t\tERROR: FILE COULD NOT BE OPENED. Press any key to continue.");
                getchar();
            }
            WIPE;
            fclose(save2);
        }break;
        case '3':{
            FILE *save3;
            char str[GRID_HEIGHT*GRID_WIDTH];
            if( (save3 = fopen("SAVE_3.txt","r"))){

                while( fgets(str, GRID_HEIGHT*GRID_WIDTH, save3) != NULL);
                short counter = 0;
                for(int i=0; i<GRID_HEIGHT; i++){
                    for(int j=0; j<GRID_WIDTH; j++){
                        if ( str[counter] == '0') grid_main[i][j] = 0;
                        else if( str[counter] == '1' ) grid_main[i][j] = 1;
                        counter++;
                    }
                }
            }
            else{
                WIPE;
                display_main_grid();
                printf( KEYBINDS "\n\t\t\t\t\t\tERROR: FILE COULD NOT BE OPENED. Press any key to continue.");
                getchar();
            }
            WIPE;
            fclose(save3);
        }break;
        default: {
            WIPE;
            display_main_grid();
            printf( KEYBINDS "\n\t\t\t\t\t\tERROR: FILE COULD NOT BE OPENED. Press any key to continue.");
            getchar();
            WIPE;
        }

    }

}

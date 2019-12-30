# Game_of_Life
The famous cellular automaton

 The C Implementation of Life

	Implementing the game of life in a computer program is as easy as it gets since the hearth of the ordeal is a set of simple 
  algorithms.

	First, let’s define two two-dimensional arrays. We will hold the data of the cells within the elements of the arrays, 0 for 
  off and 1 for on. It is easy to think about the arrays as a cartesian coordinate system where the first dimension shows the 
  y coordinate or height, dictated by custom and the second dimension shows the x coordinate or horizontal distance of the 
  cell. Picture it like so, where the first and second numbers denote the first and second dimensions of the arrays.

			0,0	0,1	0,2	0,3	0,4	0,5...
			1,0	1,1	1,2	1,3	1,4	1,5...
			2,0	2,1	2,2	2,3	2,4	2,5...
			3,0	3,1	3,2	3,3	3,4	3,5...
			4,0	4,1	4,2	4,3	4,4	4,5...
			5,0	5,1	5,2	5,3	5,4	5,5…
			.	.	.	.	.	.
			.	.	.	.	.	.

	If we print the information of the cells on the screen using two characters, displaying the grid will be done. I have 
  chosen the “white large square” unicode character to show when a cell is on, and simply two space bars to show when it’s off.

	
	After displaying the game grid we have to do calculations on the grid to iterate the information on it. This is where we
  collide with our first problem; if we do calculations on just one grid we change the outcome by altering the grid itself.
  Take a time to ponder this fact, if we calculate what the state of a cell will be in the next iteration and immediately set
  its value to reflect that we end up influencing its neighbors states by doing so. Thus, we need a second grid which will act
  as a buffer memory.

	
Doing the calculations themselves are easy enough since the rules are so simple. We can use two embedded loops to go through
the neighbors of a cell and count which ones are on. Lets say we are looking at the cell 5,6. Its neighbors are; 4,5 , 4,6 , 
4,7 , 5,5 , 5,7 , 6,5 , 6,6 and 6,7. Then we will turn the cell on if it has three alive neighbors, maintain its current 
state if it has two alive neighbors and turn it off if anything else. These are the rule set of the Conways game of life.

	After we calculate whether the cell will be on or off at the next stage, we can store that information in our second grid. 
  After we are done storing the data of all the cells we will display the new updated grid.

	But before that we have another problem: what happens at the edge of space? We can discern what the future will be for the 
  cells in the middle but on the edges there are no neighbors we can look at to calculate such information. There are 
  multiple solutions to this tricky problem. First is making space infinite, which no real computer could do. Second is 
  making an exception for the cells on the edge and keep them turned off or on no matter what. I have settled on the third 
  solution to make the space enclosed such that when you go towards one side long enough you emerge from the other like a 
  cylinder.






	Let’s look at the “calculate” function I wrote:

void calculate(){
    short neighbor_counter=0;

    for(int i=0; i<GRID_HEIGHT; i++){
        for(int j=0; j<GRID_WIDTH; j++){    

            for(int k=-1; k<=1; k++){          
                for(int l=-1; l<=1; l++){

                    if( k == 0 && l == 0) continue;	//don’t count itself
                    if ( grid_main [ mod((i+k),GRID_HEIGHT) ] [ mod((j+l),GRID_WIDTH) ] == 1) neighbor_counter++;
                }
            }

            if(  ( (neighbor_counter == 2) && (grid_main[i][j] == 1) ) || (neighbor_counter == 3)  ) grid_memory[i][j] = 1;
            else grid_memory[i][j] = 0;
            neighbor_counter = 0;                          
        }
    }

    for(int i=0; i<GRID_HEIGHT; i++){
        for(int j=0; j<GRID_WIDTH; j++) grid_main[i][j] = grid_memory[i][j];
    }
}

	GRID_WIDTH and GRID_HEIGHT are preprocessor macros which I previously defined as 100 and 50. They tell the size of the dimensions of the arrays grid_main and grid_memory.

	

	mod(a, b) is a function which returns the modulus of the two arguments. The modulus function, for example, returns 49 when the numbers -1 and 50 are plugged in. So we end up checking the cell all the way to the right while we are counting the neighbors of a cell all the way to the left which makes space enclosed.

	After all is said and done we copy the values in grid_memory to grid_main which will be displayed as graphic images on the screen later.

	Here is the display() function:

void display_main_grid(void){

    for(int i=0; i < GRID_HEIGHT; i++){
        for(int j=0; j < GRID_WIDTH; j++) {

            if( (i == 0 && j == 0)) {	//don’t pay attention here
for(int k=0; k<GRID_WIDTH*2+1; k++) printf("_"); 
printf("\n");}

            if( pause_mode == 1 && i == c.y && j == c.x )
printf(" \u2573");	
            else {grid_main[i][j] ? printf("\u2B1C") : printf("  ");}

		//don’t pay attention here either
            if (j == GRID_WIDTH - 1) printf("|\n");
            if( (i == GRID_HEIGHT-1 && j == GRID_WIDTH-1)) for(int k=0; k<GRID_WIDTH*2+1; k++) printf("\u203E");

        }
    }
}

	The function checks whether the cell it’s going through is on or off and prints the appropriate character.
  If the pause mode is on the program checks if the cursor resides on the current cell and if it does displays the cursor 
  character \u2573 on UTF-8 unicode. If the cursor is not there the program prints  \u2B1C which is another unicode character 
  if the cell is on and two spaces if not. The other stuff is just for the aesthetic look.

	After displaying the grid and the UI elements however we face the need to delete all the previous imagery before displaying 
  the updated one. There are many ways of tackling this problem, some of which leave the problem quite tackled indeed. In more 
  complex programs it is done in much less resource intensive ways like not updating the UI elements and preparing the image 
  before it is displayed. Since this is supposed to be a simple and concise program I chose to simply use an ANSI escape 
  sequence to erase every character on the screen and print them again each time.
  The escape sequence is \e[H\e[J. “\e” stands for ESC, the [H part moves the caret to the top left and [J erases the lines 
  underneath the caret. A basic solution.

	I’ve also disabled some of the default terminal settings like echoing back the input and canonical mode which takes the 
  input after you type the newline character. This way whenever the user strikes a key we can immediately take action without 
  waiting for him to hit enter button.

	The necessary functions and definitions for changing terminal settings are located in the termios.h file. It contains the 
  definition of a struct called termios which contains certain values that act kind of like a key. The terminal looks at what 
  the values inside the struct are and determines what the settings will be accordingly.
	
	The function I wrote to set a raw (non canonical) termios setting is:
struct termios raw_termios;
void enable_raw_termios_settings(void){
    tcgetattr(STDIN_FILENO, &raw_termios);
    raw_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
}

tcgetattr() function gets the parameters associated with the first argument (STDIN_FILENO is the symbol for standard input) 
and stores them in the second termios struct.
Then we disable the echo and canonical properties by making the part of the “key” which governs them 0. Lastly we set the 
new settings to our new struct raw_termios.
